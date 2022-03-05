#include "CorvusPCH.h"
#include "Window.h"

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
            CORVUS_CORE_ERROR("Attempt to re-initialize already created window!");
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

        CORVUS_CORE_INFO("Window created");

        glfwSetWindowUserPointer(m_Window, this);

        glfwSetKeyCallback(m_Window, [](GLFWwindow *window, int key, int scancode, int action, int mods)
        {
            Key::EKeyboardKey Key = static_cast<Key::EKeyboardKey>(key);
            switch (action) 
            {
            case GLFW_PRESS:
                if (Key == Key::EKeyboardKey::Escape) {
                    glfwSetWindowShouldClose(window, true);
                }
            }
        });

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
        CORVUS_CORE_INFO("Window destroyed");

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

    }

    GLFWwindow *Window::GetWindow()
    {
        return m_Window;
    }

    void Window::WindowErrorCallback(int ErrorCode, char const *Description)
    {
        CORVUS_CORE_ERROR("GLFW Window error - Code: {0}, Description: {1}", ErrorCode, static_cast<void const *>(Description));
    }

}