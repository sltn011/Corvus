#ifndef CORVUS_SOURCE_CORVUS_WINDOW_WINDOW_H
#define CORVUS_SOURCE_CORVUS_WINDOW_WINDOW_H

#include "Corvus/Core/Base.h"
#include "Corvus/Core/Delegate.h"
#include "Corvus/Event/Event.h"
#include "Corvus/GUI/GUIController.h"

#include <vulkan/vulkan.h>

struct GLFWwindow;

namespace Corvus
{

    CORVUS_DECLARE_MULTICAST_DELEGATE(COnEventDelegate, CEvent &);

    struct SWindowSettings
    {
        CString WindowName;
        Int32   WindowWidth  = 0;
        Int32   WindowHeight = 0;

        bool bFullScreen = false;
    };

    struct SWindowData
    {
        CString         WindowName;
        SWindowSettings WindowSettings;
    };

    struct SWindowInitInfo
    {
        CString         WindowName;
        UInt32          WindowWidth  = 0;
        UInt32          WindowHeight = 0;
        SWindowSettings WindowSettings;
    };

    class CWindow
    {
    protected:
        CWindow() = default;

    public:
        static [[nodiscard]] TOwn<CWindow> Create();

        virtual ~CWindow()                  = default;
        CWindow(CWindow const &)            = delete;
        CWindow &operator=(CWindow const &) = delete;
        CWindow(CWindow &&)                 = default;
        CWindow &operator=(CWindow &&)      = default;

        virtual void Init(SWindowData const &Settings) = 0;
        virtual void CreateGUIController()             = 0;
        virtual void DestroyGUIController()            = 0;

        virtual void OnUpdate() = 0;

        bool IsInitialized() const { return m_bIsInitialized; }

        CString GetWindowName() const { return m_WindowData.WindowName; }

        virtual std::pair<UInt32, UInt32> GetFramebufferSize() const = 0;

        virtual bool ShouldClose() const = 0;
        virtual void SetShouldClose()    = 0;

        bool         IsFullScreen() const;
        virtual void SetFullScreen(bool bValue) = 0;

        virtual void AwaitNextEvent() const = 0;

        virtual void *GetRawWindow() = 0;
        
        virtual std::vector<char const *> GetRequiredExtensions() = 0;

        virtual VkSurfaceKHR CreateVulkanSurfaceHandler() const = 0;

        CGUIController &GetGUIController();

        COnEventDelegate OnEvent;

    protected:
        SWindowData m_WindowData;
        bool        m_bIsInitialized = false;

        CGUIController m_GUIController;
    };

} // namespace Corvus

#endif //! CORVUS_SOURCE_CORVUS_WINDOW_WINDOW_H