#include "CorvusPCH.h"

#include "Corvus/Window/Window.h"

#include "Platform/Windows/Window/WindowsWindow.h"

namespace Corvus
{
    TOwn<CWindow> CWindow::Create()
    {
        return MakeOwned<PWindowsWindow>();
    }

    UInt32 CWindow::GetWindowWidth() const
    {
        return GetWindowSize().x;
    }

    UInt32 CWindow::GetWindowHeight() const
    {
        return GetWindowSize().y;
    }

    bool CWindow::IsVSyncEnabled() const
    {
        if (!m_bIsInitialized)
        {
            return false;
        }

        return m_WindowData.WindowSettings.bVSyncEnabled;
    }

    bool CWindow::IsFullScreen() const
    {
        if (!m_bIsInitialized)
        {
            return false;
        }

        return m_WindowData.WindowSettings.bFullScreen;
    }

} // namespace Corvus