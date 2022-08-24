#ifndef CORVUS_SOURCE_CORVUS_WINDOW_WINDOW_H
#define CORVUS_SOURCE_CORVUS_WINDOW_WINDOW_H

#include "Corvus/Core/Base.h"
#include "Corvus/Core/Delegate.h"
#include "Corvus/Events/Event.h"
#include "Corvus/GUI/GUIController.h"
#include "Corvus/Renderer/RenderingContext.h"

struct GLFWwindow;

namespace Corvus
{

    CORVUS_DECLARE_MULTICAST_DELEGATE(COnEventDelegate, СEvent &);

    struct CWindowData
    {
        CString WindowName;
        Int32   WindowWidth  = 0;
        Int32   WindowHeight = 0;

        bool bVSyncEnabled = false;
        bool bFullScreen   = false;
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

        virtual void Init(CWindowData const &Settings) = 0;
        virtual void InitRenderingContext()            = 0;
        virtual void InitGUIRenderingContext()         = 0;

        virtual void OnUpdate() = 0;

        bool    IsInitialized() const { return m_bIsInitialized; }
        UInt32  GetWindowWidth() const { return m_WindowData.WindowWidth; }
        UInt32  GetWindowHeight() const { return m_WindowData.WindowHeight; }
        CString GetWindowName() const { return m_WindowData.WindowName; }

        virtual bool ShouldClose() const = 0;
        virtual void SetShouldClose()    = 0;

        bool         IsVSyncEnabled() const;
        virtual void SetVSyncEnabled(bool bValue) = 0;

        bool         IsFullScreen() const;
        virtual void SetFullScreen(bool bValue) = 0;

        virtual void *GetRawWindow() = 0;

        СGUIController &GetGUIController();

        COnEventDelegate OnEvent;

    protected:
        CWindowData m_WindowData;
        bool        m_bIsInitialized = false;

        TOwn<CRenderingContext> m_RenderingContext;
        СGUIController          m_GUIController;
    };

} // namespace Corvus

#endif //! CORVUS_SOURCE_CORVUS_WINDOW_WINDOW_H