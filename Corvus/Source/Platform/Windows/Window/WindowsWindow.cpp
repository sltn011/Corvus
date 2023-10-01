#include "CorvusPCH.h"

#include "Platform/Windows/Window/WindowsWindow.h"

#include "Corvus/Event/ApplicationEvent.h"
#include "Corvus/Event/KeyboardEvent.h"
#include "Corvus/Event/MouseEvent.h"
#include "Corvus/Renderer/Renderer.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace Corvus
{
    Int8 PWindowsWindow::s_WindowsCount = 0;

    PWindowsWindow::PWindowsWindow() : m_Window{nullptr}
    {
    }

    PWindowsWindow::~PWindowsWindow()
    {
        Destroy();
    }

    void PWindowsWindow::Init(SWindowData const &Settings)
    {
        CORVUS_CORE_ASSERT_FMT(
            !m_bIsInitialized, "Can not re-initialize already created window \"{0}\"!", m_WindowData.WindowName
        );

        if (s_WindowsCount == 0)
        {
            bool const GLFWInitialized = glfwInit();
            CORVUS_CORE_ASSERT(GLFWInitialized);
            CORVUS_CORE_TRACE("GLFW initialized successfully");

            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

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

        // InitGUIRenderingContext();

        glfwSetWindowUserPointer(m_Window, this);

        SetFullScreen(m_WindowData.bFullScreen);

        CORVUS_CORE_ASSERT(m_Window);
    }

    void PWindowsWindow::Destroy()
    {
        if (!m_bIsInitialized)
        {
            CORVUS_CORE_TRACE("Window is not initialized and does not need to be destroyed");
            return;
        }

        // m_GUIController.Destroy();

        glfwDestroyWindow(m_Window);
        m_Window         = nullptr;
        m_bIsInitialized = false;
        CORVUS_CORE_INFO("Window \"{0}\" destroyed", m_WindowData.WindowName);

        --s_WindowsCount;
        if (!s_WindowsCount)
        {
            glfwTerminate();
            CORVUS_CORE_TRACE("GLFW terminated");
        }
    }

    void PWindowsWindow::OnUpdate()
    {
        CORVUS_CORE_ASSERT(m_bIsInitialized);
        glfwPollEvents();
    }

    std::pair<UInt32, UInt32> PWindowsWindow::GetFramebufferSize() const
    {
        std::pair<Int32, Int32> FramebufferSize;
        glfwGetFramebufferSize(m_Window, &FramebufferSize.first, &FramebufferSize.second);
        return std::pair<UInt32, UInt32>{
            static_cast<UInt32>(FramebufferSize.first), static_cast<UInt32>(FramebufferSize.second)};
    }

    bool PWindowsWindow::ShouldClose() const
    {
        return glfwWindowShouldClose(m_Window);
    }

    void PWindowsWindow::SetShouldClose()
    {
        glfwSetWindowShouldClose(m_Window, true);
    }

    void PWindowsWindow::SetFullScreen(bool const bValue)
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
                0,
                0,
                m_WindowData.WindowWidth,
                m_WindowData.WindowHeight,
                GLFW_DONT_CARE
            );
        }
        else
        {
            glfwSetWindowMonitor(
                m_Window, nullptr, 0, 0, m_WindowData.WindowWidth, m_WindowData.WindowHeight, GLFW_DONT_CARE
            );

            FIntVector2 MonitorTopLeft{}, MonitorBottomRight{};
            glfwGetMonitorWorkarea(
                glfwGetPrimaryMonitor(),
                &MonitorTopLeft.x,
                &MonitorTopLeft.y,
                &MonitorBottomRight.x,
                &MonitorBottomRight.y
            );

            FIntVector2 const MonitorCenter = (MonitorBottomRight - MonitorTopLeft) / 2;

            FIntVector2 WindowTopLeft;
            WindowTopLeft.x = FMath::Max(0, MonitorCenter.x - m_WindowData.WindowWidth / 2);
            WindowTopLeft.y = FMath::Max(0, MonitorCenter.y - m_WindowData.WindowHeight / 2);

            glfwSetWindowPos(m_Window, WindowTopLeft.x, WindowTopLeft.y);
        }

        m_WindowData.bFullScreen = bValue;
        CORVUS_CORE_TRACE("Window \"{0}\" FullScreen {1}", m_WindowData.WindowName, bValue ? "On" : "Off");
    }

    void PWindowsWindow::AwaitNextEvent() const
    {
        glfwWaitEvents();
    }

    void *PWindowsWindow::GetRawWindow()
    {
        return m_Window;
    }

    std::vector<char const *> PWindowsWindow::GetRequiredExtensions()
    {
        UInt32                    NumExtensions;
        char const              **Extensions = glfwGetRequiredInstanceExtensions(&NumExtensions);
        std::vector<char const *> RequiredExtensionsVec(NumExtensions);
        for (UInt32 i = 0; i < NumExtensions; ++i)
        {
            RequiredExtensionsVec[i] = Extensions[i];
        }
        return RequiredExtensionsVec;
    }

    VkSurfaceKHR PWindowsWindow::CreateVulkanSurfaceHandler() const
    {
        VkSurfaceKHR SurfaceHandler = VK_NULL_HANDLE;
        if (glfwCreateWindowSurface(Renderer().GetVulkanInstance(), m_Window, nullptr, &SurfaceHandler) != VK_SUCCESS)
        {
            CORVUS_CRITICAL("Failed to create VkSurface!");
        }
        CORVUS_TRACE("Created VkSurface successfully");
        return SurfaceHandler;
    }

    void PWindowsWindow::WindowErrorCallback(int const ErrorCode, char const *const Description)
    {
        CORVUS_CORE_ERROR("GLFW Error - Code: {0}, Description: {1:s}", ErrorCode, Description);
    }

    /*void PWindowsWindow::InitGUIRenderingContext()
    {
        m_GUIController.Init();
        CORVUS_CORE_INFO("GUI rendering context created");
    }*/

    void PWindowsWindow::SetupWindowEventsHandlers()
    {
        glfwSetWindowSizeCallback(
            m_Window,
            [](GLFWwindow *const Caller, int const NewWidth, int const NewHeight)
            {
                CWindow const *const Owner = static_cast<CWindow *>(glfwGetWindowUserPointer(Caller));
                CWindowResizeEvent   Event{NewWidth, NewHeight};
                Owner->OnEvent.Broadcast(Event);
            }
        );

        glfwSetWindowCloseCallback(
            m_Window,
            [](GLFWwindow *const Caller)
            {
                CWindow const *const Owner = static_cast<CWindow *>(glfwGetWindowUserPointer(Caller));
                CWindowCloseEvent    Event{};
                Owner->OnEvent.Broadcast(Event);
            }
        );

        glfwSetKeyCallback(
            m_Window,
            [](GLFWwindow *const Caller, int const RawKey, int const RawScancode, int const RawAction, int const RawMods
            )
            {
                CWindow const *const Owner = static_cast<CWindow *>(glfwGetWindowUserPointer(Caller));

                EKeyCode const      Key    = static_cast<EKeyCode>(RawKey);
                EActionCode const   Action = static_cast<EActionCode>(RawAction);
                EModifierCode const Mods   = static_cast<EModifierCode>(RawMods);

                if (Action == Action::Press)
                {
                    CKeyPressEvent Event{Key, false, Mods};
                    Owner->OnEvent.Broadcast(Event);
                    return;
                }
                else if (Action == Action::Repeat)
                {
                    CKeyPressEvent Event{Key, true, Mods};
                    Owner->OnEvent.Broadcast(Event);
                    return;
                }
                else if (Action == Action::Release)
                {
                    CKeyReleaseEvent Event{Key};
                    Owner->OnEvent.Broadcast(Event);
                    return;
                }
            }
        );

        glfwSetMouseButtonCallback(
            m_Window,
            [](GLFWwindow *const Caller, int const RawButton, int const RawAction, int const RawMods)
            {
                CWindow const *const Owner = static_cast<CWindow *>(glfwGetWindowUserPointer(Caller));

                EMouseCode const    Button = static_cast<EMouseCode>(RawButton);
                EActionCode const   Action = static_cast<EActionCode>(RawAction);
                EModifierCode const Mods   = static_cast<EModifierCode>(RawMods);

                if (Action == Action::Press)
                {
                    CMouseButtonPressEvent Event{Button, Mods};
                    Owner->OnEvent.Broadcast(Event);
                    return;
                }
                else if (Action == Action::Release)
                {
                    CMouseButtonReleaseEvent Event{Button};
                    Owner->OnEvent.Broadcast(Event);
                    return;
                }
            }
        );

        glfwSetCursorPosCallback(
            m_Window,
            [](GLFWwindow *const Caller, double const NewX, double const NewY)
            {
                CWindow const *const Owner = static_cast<CWindow *>(glfwGetWindowUserPointer(Caller));
                CCursorMoveEvent     Event{static_cast<float>(NewX), static_cast<float>(NewY)};
                Owner->OnEvent(Event);
            }
        );

        glfwSetScrollCallback(
            m_Window,
            [](GLFWwindow *const Caller, double const OffsetX, double const OffsetY)
            {
                CWindow const *const Owner = static_cast<CWindow *>(glfwGetWindowUserPointer(Caller));
                CMouseScrollEvent    Event{static_cast<float>(OffsetX), static_cast<float>(OffsetY)};
                Owner->OnEvent(Event);
            }
        );

        CORVUS_CORE_TRACE("Window \"{0}\" event handlers created", m_WindowData.WindowName);
    }

} // namespace Corvus