#include "CorvusPCH.h"
#include "Corvus/Events/ApplicationEvent.h"
#include "Corvus/Events/KeyboardEvent.h"
#include "Corvus/Events/MouseEvent.h"
#include "Platform/Windows/WindowsWindow.h"

#include <GLFW/glfw3.h>

namespace Corvus
{
    Int8 WindowsWindow::s_WindowsCount = 0;

    WindowsWindow::WindowsWindow()
        : m_Window{ nullptr }
    {
    }

    WindowsWindow::~WindowsWindow()
    {
        Destroy();
    }

    void WindowsWindow::Init(WindowData const &Settings)
    {
        CORVUS_CORE_ASSERT_FMT(!m_bIsInitialized, "Can not re-initialize already created window \"{0}\"!", m_WindowData.WindowName);

        if (s_WindowsCount == 0)
        {
            bool const GLFWInitialized = glfwInit();
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

        SetupWindowEventsHandlers(); // Must be called before InitGUIRenderingContext
        
        InitRenderingContext();
        InitGUIRenderingContext();

        glfwSetWindowUserPointer(m_Window, this);

        SetFullScreen(m_WindowData.bFullScreen);
        SetVSyncEnabled(m_WindowData.bVSyncEnabled);

        CORVUS_CORE_ASSERT(m_Window);
        CORVUS_CORE_ASSERT(m_RenderingContext);
    }

    void WindowsWindow::Destroy()
    {
        if (!m_bIsInitialized)
        {
            CORVUS_CORE_TRACE("Window is not initialized and does not need to be destroyed");
            return;
        }

        m_RenderingContext.reset();
        m_GUIController.Destroy();

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

    void WindowsWindow::OnUpdate()
    {
        CORVUS_CORE_ASSERT(m_bIsInitialized);
        glfwPollEvents();
        m_RenderingContext->SwapBuffers();
    }

    bool WindowsWindow::ShouldClose() const
    {
        return glfwWindowShouldClose(m_Window);
    }

    void WindowsWindow::SetShouldClose()
    {
        glfwSetWindowShouldClose(m_Window, true);
    }

    void WindowsWindow::SetVSyncEnabled(bool bValue)
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

    void WindowsWindow::SetFullScreen(bool bValue)
    {
        if (!m_bIsInitialized)
        {
            CORVUS_CORE_ERROR("Window not initialized - cant switch FullScreen on/off!");
            return;
        }

        if (bValue)
        {
            glfwSetWindowMonitor(
                m_Window,
                glfwGetPrimaryMonitor(),
                0, 0, m_WindowData.WindowWidth, m_WindowData.WindowHeight,
                GLFW_DONT_CARE
            );
        }
        else
        {
            glfwSetWindowMonitor(
                m_Window,
                nullptr,
                0, 0, m_WindowData.WindowWidth, m_WindowData.WindowHeight,
                GLFW_DONT_CARE
            );

            IVec2 MonitorTopLeft, MonitorBottomRight;
            glfwGetMonitorWorkarea(
                glfwGetPrimaryMonitor(),
                &MonitorTopLeft.x, &MonitorTopLeft.y,
                &MonitorBottomRight.x, &MonitorBottomRight.y
            );

            IVec2 const MonitorCenter = (MonitorBottomRight - MonitorTopLeft) / 2;

            IVec2 WindowTopLeft;
            WindowTopLeft.x = Math::Max(0, MonitorCenter.x - m_WindowData.WindowWidth / 2);
            WindowTopLeft.y = Math::Max(0, MonitorCenter.y - m_WindowData.WindowHeight / 2);

            glfwSetWindowPos(m_Window, WindowTopLeft.x, WindowTopLeft.y);
        }

        m_WindowData.bFullScreen = bValue;
        CORVUS_CORE_TRACE("Window \"{0}\" FullScreen {1}", m_WindowData.WindowName, bValue ? "On" : "Off");
    }

    void *WindowsWindow::GetRawWindow()
    {
        return m_Window;
    }

    void WindowsWindow::WindowErrorCallback(int ErrorCode, char const *Description)
    {
        CORVUS_CORE_ERROR("GLFW Error - Code: {0}, Description: {1:s}", ErrorCode, Description);
    }

    void WindowsWindow::InitRenderingContext()
    {
        m_RenderingContext = RenderingContext::Create(*this);
        CORVUS_CORE_ASSERT(m_RenderingContext);
        m_RenderingContext->Init();
        CORVUS_CORE_INFO("Rendering context created");
    }

    void WindowsWindow::InitGUIRenderingContext()
    {
        m_GUIController.Init();
        CORVUS_CORE_INFO("GUI rendering context created");
    }

    void WindowsWindow::SetupWindowEventsHandlers()
    {
        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow *Caller, int NewWidth, int NewHeight)
        {
            Window const *const Owner = static_cast<Window *>(glfwGetWindowUserPointer(Caller));
            WindowResizeEvent Event{ NewWidth, NewHeight };
            Owner->OnEvent.Broadcast(Event);
        });

        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow *Caller)
        {
            Window const *const Owner = static_cast<Window *>(glfwGetWindowUserPointer(Caller));
            WindowCloseEvent Event{};
            Owner->OnEvent.Broadcast(Event);
        });

        glfwSetKeyCallback(m_Window, [](GLFWwindow *Caller, int RawKey, int RawScancode, int RawAction, int RawMods)
        {
            Window const *const Owner = static_cast<Window *>(glfwGetWindowUserPointer(Caller));

            KeyCode      const Key    = static_cast<KeyCode>(RawKey);
            ActionCode   const Action = static_cast<ActionCode>(RawAction);
            ModifierCode const Mods   = static_cast<ModifierCode>(RawMods);

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
            Window const *const Owner = static_cast<Window *>(glfwGetWindowUserPointer(Caller));

            MouseCode    const Button = static_cast<MouseCode>(RawButton);
            ActionCode   const Action = static_cast<ActionCode>(RawAction);
            ModifierCode const Mods   = static_cast<ModifierCode>(RawMods);

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
            Window const *const Owner = static_cast<Window *>(glfwGetWindowUserPointer(Caller));
            CursorMoveEvent Event{ static_cast<float>(NewX), static_cast<float>(NewY) };
            Owner->OnEvent(Event);
        });

        glfwSetScrollCallback(m_Window, [](GLFWwindow *Caller, double OffsetX, double OffsetY)
        {
            Window const *const Owner = static_cast<Window *>(glfwGetWindowUserPointer(Caller));
            MouseScrollEvent Event{ static_cast<float>(OffsetX), static_cast<float>(OffsetY) };
            Owner->OnEvent(Event);
        });

        CORVUS_CORE_TRACE("Window \"{0}\" event handlers created", m_WindowData.WindowName);
    }

}