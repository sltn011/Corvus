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
        UInt32 WindowWidth;
        UInt32 WindowHeight;

        bool  bVSyncEnabled = false;
        bool  bFullScreen = false;
    };

    class Window
    {
    public:

        Window();
        ~Window();

        bool Init(WindowData const &Settings);
        void Destroy();

        void OnUpdate();

        bool IsInitialized() const { return m_bIsInitialized; }
        UInt32 GetWindowWidth() const { return m_WindowData.WindowWidth; }
        UInt32 GetWindowHeight() const { return m_WindowData.WindowHeight; }
        String GetWindowName() const { return m_WindowData.WindowName; }

        bool IsVSyncEnabled() const;
        void SetVSyncEnabled(bool bValue);

        bool IsFullScreen() const;
        void SetFullScreen(bool bValue);

        GLFWwindow *GetRawWindow();

        static void WindowErrorCallback(int ErrorCode, char const *Description);

        OnEventDelegate OnEvent;

    protected:

        void SetupWindowEventsHandlers();

        GLFWwindow *m_Window;
        WindowData  m_WindowData;

    private:

        bool m_bIsInitialized;

        static Int8 s_WindowsCount;
    };

}

#endif //!CORVUS_SOURCE_CORVUS_WINDOW_WINDOW_H