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

        virtual void Init(SWindowInitInfo const &InitInfo) override;

        void Destroy();

        virtual void OnUpdate() override;

        virtual std::pair<UInt32, UInt32> GetFramebufferSize() const override;

        virtual bool ShouldClose() const override;
        virtual void SetShouldClose() override;

        virtual void SetFullScreen(bool bValue) override;

        virtual void AwaitNextEvent() const override;

        virtual void *GetRawWindow() override;

        virtual std::vector<char const *> GetRequiredExtensions() override;

        virtual VkSurfaceKHR CreateVulkanSurfaceHandler() const override;

        static void WindowErrorCallback(int ErrorCode, char const *Description);

    protected:
        virtual void CreateGUIController() override;
        virtual void DestroyGUIController() override;

        void SetupWindowEventsHandlers();

        GLFWwindow *m_Window;

    private:
        static Int8 s_WindowsCount;
    };
} // namespace Corvus

#endif // !CORVUS_SOURCE_PLATFORM_WINDOWS_WINDOW_WINDOWSWINDOW_H