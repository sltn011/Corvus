#ifndef CORVUS_SOURCE_PLATFORM_WINDOWS_WINDOWSWINDOW_H
#define CORVUS_SOURCE_PLATFORM_WINDOWS_WINDOWSWINDOW_H

#include "Corvus/Window/Window.h"

namespace Corvus
{
    class WindowsWindow : public Window
    {
    public:

        WindowsWindow();
        virtual ~WindowsWindow();

        virtual void Init(WindowData const &Settings) override;
        virtual void InitRenderingContext() override;
        virtual void InitGUIRenderingContext() override;

        void Destroy(); 

        virtual void OnUpdate() override;

        virtual bool ShouldClose() const override;
        virtual void SetShouldClose() override;

        virtual void SetVSyncEnabled(bool bValue) override;
        virtual void SetFullScreen(bool bValue) override;

        virtual void *GetRawWindow() override;

        static void WindowErrorCallback(int ErrorCode, char const *Description);

    protected:

        void SetupWindowEventsHandlers();

        GLFWwindow *m_Window;

    private:

        static Int8 s_WindowsCount;
    };
}

#endif // !CORVUS_SOURCE_PLATFORM_WINDOWS_WINDOWSWINDOW_H