#ifndef CORVUS_SOURCE_CORVUS_WINDOW_WINDOW_H
#define CORVUS_SOURCE_CORVUS_WINDOW_WINDOW_H

#include "Corvus/Core/Base.h"
#include "Corvus/Core/Delegate.h"
#include "Corvus/Event/Event.h"
#include "Corvus/Math/Vector.h"
#include "Corvus/Renderer/RenderingContext.h"

struct GLFWwindow;

namespace Corvus
{

    CORVUS_DECLARE_MULTICAST_DELEGATE(COnEventDelegate, CEvent &);

    struct SWindowSettings
    {
        bool bVSyncEnabled = false;
        bool bFullScreen   = false;
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

        virtual void Init(SWindowInitInfo const &InitInfo) = 0;
        virtual void InitRenderingContext()                = 0;

        virtual void OnUpdate() = 0;

        bool IsInitialized() const { return m_bIsInitialized; }

        CString GetWindowName() const { return m_WindowData.WindowName; }

        // In pixels
        virtual FUIntVector2 GetWindowSize() const = 0;
        UInt32               GetWindowWidth() const;
        UInt32               GetWindowHeight() const;

        virtual bool ShouldClose() const = 0;
        virtual void SetShouldClose()    = 0;

        bool         IsVSyncEnabled() const;
        virtual void SetVSyncEnabled(bool bValue) = 0;

        bool         IsFullScreen() const;
        virtual void SetFullScreen(bool bValue) = 0;

        virtual void *GetRawWindow() = 0;

        COnEventDelegate OnEvent;

    protected:
        SWindowData m_WindowData;
        bool        m_bIsInitialized = false;

        TOwn<CRenderingContext> m_RenderingContext;
    };

} // namespace Corvus

#endif //! CORVUS_SOURCE_CORVUS_WINDOW_WINDOW_H