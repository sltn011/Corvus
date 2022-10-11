#include "CorvusPCH.h"

#include "Corvus/Window/Window.h"

#include "Platform/Windows/Window/WindowsWindow.h"

namespace Corvus
{
    TOwn<CWindow> CWindow::Create()
    {
        return MakeOwned<PWindowsWindow>();
    }

    bool CWindow::IsVSyncEnabled() const
    {
        if (!m_bIsInitialized)
        {
            return false;
        }

        return m_WindowData.bVSyncEnabled;
    }

    bool CWindow::IsFullScreen() const
    {
        if (!m_bIsInitialized)
        {
            return false;
        }

        return m_WindowData.bFullScreen;
    }

    CGUIController &CWindow::GetGUIController()
    {
        return m_GUIController;
    }
} // namespace Corvus