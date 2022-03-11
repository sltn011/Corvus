#include "CorvusPCH.h"
#include "Corvus/Window/Window.h"

#include "Platform/Windows/WindowsWindow.h"

namespace Corvus
{
    Own<Window> Window::Create()
    {
        return MakeOwned<WindowsWindow>();
    }

    Window::Window()
        : m_bIsInitialized{ false }
    {
    }

    Window::~Window()
    {
    }

    bool Window::IsVSyncEnabled() const
    {
        if (!m_bIsInitialized)
        {
            return false;
        }

        return m_WindowData.bVSyncEnabled;
    }

    bool Window::IsFullScreen() const
    {
        if (!m_bIsInitialized)
        {
            return false;
        }

        return m_WindowData.bFullScreen;
    }
}