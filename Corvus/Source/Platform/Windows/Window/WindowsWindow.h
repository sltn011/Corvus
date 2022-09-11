#ifndef CORVUS_SOURCE_PLATFORM_WINDOWS_WINDOW_WINDOWSWINDOW_H
#define CORVUS_SOURCE_PLATFORM_WINDOWS_WINDOW_WINDOWSWINDOW_H

#include "Corvus/Window/Window.h"

namespace Corvus
{
    class PWindowsWindow : public CWindow
    {
    public:
        using Super = CWindow;

        PWindowsWindow();
        ~PWindowsWindow();
        PWindowsWindow(PWindowsWindow const &)            = delete;
        PWindowsWindow &operator=(PWindowsWindow const &) = delete;
        PWindowsWindow(PWindowsWindow &&)                 = default;
        PWindowsWindow &operator=(PWindowsWindow &&)      = default;

        virtual void Init(SWindowData const &Settings) override;

        void Destroy();

        virtual void OnUpdate() override;

        virtual bool ShouldClose() const override;
        virtual void SetShouldClose() override;

        virtual void SetVSyncEnabled(bool bValue) override;
        virtual void SetFullScreen(bool bValue) override;

        virtual void *GetRawWindow() override;

        static void WindowErrorCallback(int ErrorCode, char const *Description);

    protected:
        virtual void InitRenderingContext() override;
        virtual void InitGUIRenderingContext() override;

        void SetupWindowEventsHandlers();

        GLFWwindow *m_Window;

    private:
        static Int8 s_WindowsCount;
    };
} // namespace Corvus

#endif // !CORVUS_SOURCE_PLATFORM_WINDOWS_WINDOW_WINDOWSWINDOW_H