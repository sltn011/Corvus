#include "CorvusPCH.h"
#include "Corvus/Events/ApplicationEvent.h"
#include "Corvus/Events/KeyboardEvent.h"
#include "Corvus/Events/MouseEvent.h"
#include "Corvus/Window/Window.h"

#include <GLFW/glfw3.h>

namespace Corvus
{
    Int8 Window::s_WindowsCount = 0;

    Window::Window()
        : m_Window { nullptr }, m_bIsInitialized{ false }
    {
    }

    Window::~Window()
    {
        Destroy();
    }

    bool Window::Init(WindowData const &Settings)
    {
        if (m_bIsInitialized)
        {
            CORVUS_CORE_ERROR("Attempt to re-initialize already created window \"{0}\"!", m_WindowData.WindowName);
            return false;
        }

        if (!s_WindowsCount) 
        {
            bool GLFWInitialized = glfwInit();
            CORVUS_CORE_ASSERT(GLFWInitialized);
            CORVUS_CORE_TRACE("GLFW initialized successfully");

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

            glfwSetErrorCallback(WindowErrorCallback);
        }

        m_WindowData = Settings;

        m_Window = glfwCreateWindow(
            static_cast<int>(m_WindowData.WindowWidth),
            static_cast<int>(m_WindowData.WindowHeight),
            m_WindowData.WindowName.c_str(),
            m_WindowData.bFullScreen ? glfwGetPrimaryMonitor() : nullptr,
            nullptr
        );

        m_bIsInitialized = true;
        ++s_WindowsCount;

        if (!m_Window)
        {
            Destroy();
            CORVUS_NO_ENTRY_FMT("Failed to create window!");
        }

        CORVUS_CORE_INFO("Window \"{0}\" created", m_WindowData.WindowName);

        glfwSetWindowUserPointer(m_Window, this);

        SetupWindowEventsHandlers();

        return true;
    }

    void Window::Destroy()
    {
        if (!m_bIsInitialized)
        {
            CORVUS_CORE_TRACE("Window is not initialized and does not need to be destroyed");
            return;
        }

        glfwDestroyWindow(m_Window);
        m_Window = nullptr;
        m_bIsInitialized = false;
        CORVUS_CORE_INFO("Window \"{0}\" destroyed", m_WindowData.WindowName);

        --s_WindowsCount;
        if (!s_WindowsCount)
        {
            glfwTerminate();
            CORVUS_CORE_TRACE("GLFW terminated");
        }
    }

    void Window::OnUpdate()
    {
        CORVUS_CORE_ASSERT(m_bIsInitialized);
        glfwPollEvents();
    }

    bool Window::IsVSyncEnabled() const
    {
        if (!m_bIsInitialized)
        {
            return false;
        }

        return m_WindowData.bVSyncEnabled;
    }

    void Window::SetVSyncEnabled(bool bValue)
    {
        if (!m_bIsInitialized)
        {
            CORVUS_CORE_ERROR("Window not initialized - cant switch VSync on/off!");
            return;
        }

        glfwSwapInterval(bValue ? 1 : 0);
        m_WindowData.bVSyncEnabled = bValue;
        CORVUS_CORE_TRACE("Window \"{0}\" VSync {1}", m_WindowData.WindowName, bValue ? "On" : "Off");
    }

    bool Window::IsFullScreen() const
    {
        if (!m_bIsInitialized)
        {
            return false;
        }

        return m_WindowData.bFullScreen;
    }

    void Window::SetFullScreen(bool bValue)
    {
        CORVUS_CORE_ERROR("Changing window fullscreen mode not implemented yet!");
    }

    void *Window::GetRawWindow()
    {
        return m_Window;
    }

    void Window::WindowErrorCallback(int ErrorCode, char const *Description)
    {
        CORVUS_CORE_ERROR("GLFW Error - Code: {0}, Description: {1}", ErrorCode, static_cast<void const *>(Description));
    }

    void Window::SetupWindowEventsHandlers()
    {
        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow *Caller, int NewWidth, int NewHeight)
        {
            Window *Owner = static_cast<Window *>(glfwGetWindowUserPointer(Caller));
            WindowResizeEvent Event{ NewWidth, NewHeight };
            Owner->OnEvent.Broadcast(Event);
        });

        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow *Caller)
        {
            Window *Owner = static_cast<Window *>(glfwGetWindowUserPointer(Caller));
            WindowCloseEvent Event{};
            Owner->OnEvent.Broadcast(Event);
        });

        glfwSetKeyCallback(m_Window, [](GLFWwindow *Caller, int RawKey, int RawScancode, int RawAction, int RawMods)
        {
            Window *Owner = static_cast<Window *>(glfwGetWindowUserPointer(Caller));
            
            KeyCode Key = static_cast<KeyCode>(RawKey);
            ActionCode Action = static_cast<ActionCode>(RawAction);
            ModifierCode Mods = static_cast<ModifierCode>(RawMods);

            if (Action == Action::Press) 
            {
                KeyPressEvent Event{ Key, false, Mods };
                Owner->OnEvent.Broadcast(Event);
                return;
            }
            else if (Action == Action::Repeat)
            {
                KeyPressEvent Event{ Key, true, Mods };
                Owner->OnEvent.Broadcast(Event);
                return;
            }
            else if (Action == Action::Release)
            {
                KeyReleaseEvent Event{ Key };
                Owner->OnEvent.Broadcast(Event);
                return;
            }
        });

        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow *Caller, int RawButton, int RawAction, int RawMods)
        {
            Window *Owner = static_cast<Window *>(glfwGetWindowUserPointer(Caller));

            MouseCode Button = static_cast<MouseCode>(RawButton);
            ActionCode Action = static_cast<ActionCode>(RawAction);
            ModifierCode Mods = static_cast<ModifierCode>(RawMods);

            if (Action == Action::Press)
            {
                MouseButtonPressEvent Event{ Button, Mods };
                Owner->OnEvent.Broadcast(Event);
                return;
            }
            else if (Action == Action::Release)
            {
                MouseButtonReleaseEvent Event{ Button };
                Owner->OnEvent.Broadcast(Event);
                return;
            }
        });

        glfwSetCursorPosCallback(m_Window, [](GLFWwindow *Caller, double NewX, double NewY)
        {
            Window *Owner = static_cast<Window *>(glfwGetWindowUserPointer(Caller));
            CursorMoveEvent Event{ static_cast<float>(NewX), static_cast<float>(NewY) };
            Owner->OnEvent(Event);
        });

        glfwSetScrollCallback(m_Window, [](GLFWwindow *Caller, double OffsetX, double OffsetY)
        {
            Window *Owner = static_cast<Window *>(glfwGetWindowUserPointer(Caller));
            MouseScrollEvent Event{ static_cast<float>(OffsetX), static_cast<float>(OffsetY) };
            Owner->OnEvent(Event);
        });

        CORVUS_CORE_TRACE("Window \"{0}\" event handlers created", m_WindowData.WindowName);
    }

}