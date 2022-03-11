#ifndef CORVUS_SOURCE_CORVUS_WINDOW_WINDOW_H
#define CORVUS_SOURCE_CORVUS_WINDOW_WINDOW_H

#include "Corvus/Core/Base.h"
#include "Corvus/Core/Delegate.h"
#include "Corvus/Events/EventBase.h"

struct GLFWwindow;

namespace Corvus
{

    CORVUS_DECLARE_MULTICAST_DELEGATE(OnEventDelegate, EventBase&);

    struct WindowData
    {
        String WindowName;
        UInt32 WindowWidth = 0;
        UInt32 WindowHeight = 0;

        bool  bVSyncEnabled = false;
        bool  bFullScreen = false;
    };

    class Window
    {
    public:

        static Own<Window> Create();

        Window();
        virtual ~Window();

        virtual void Init(WindowData const &Settings) = 0;

        virtual void OnUpdate() = 0;

        bool IsInitialized() const { return m_bIsInitialized; }
        UInt32 GetWindowWidth() const { return m_WindowData.WindowWidth; }
        UInt32 GetWindowHeight() const { return m_WindowData.WindowHeight; }
        String GetWindowName() const { return m_WindowData.WindowName; }

        bool IsVSyncEnabled() const;
        virtual void SetVSyncEnabled(bool bValue) = 0;

        bool IsFullScreen() const;
        virtual void SetFullScreen(bool bValue) = 0;

        virtual void *GetRawWindow() = 0;

        OnEventDelegate OnEvent;

    protected:

        WindowData  m_WindowData;

        bool m_bIsInitialized;

    };

}

#endif //!CORVUS_SOURCE_CORVUS_WINDOW_WINDOW_H