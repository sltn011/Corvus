#ifndef CORVUS_SOURCE_CORVUS_CORE_APPLICATION_H
#define CORVUS_SOURCE_CORVUS_CORE_APPLICATION_H

#include "Corvus/Core/Base.h"
#include "Corvus/Core/LayersStack.h"
#include "Corvus/GUI/GUIController.h"
#include "Corvus/Window/Window.h"

namespace Corvus
{
    class CApplication;

    struct SApplicationCreateInfo
    {
        CString ApplicationName;
        CString ApplicationVersion;

        UInt32 ApplicationWindowWidth;
        UInt32 ApplicationWindowHeight;
    };

    // To be defined by Client
    CApplication *CreateApplication();
    bool          DestroyApplication(CApplication *App);
    //=========================================

    class FTimeDelta;

    class CApplication
    {
    public:
        CApplication(SApplicationCreateInfo const &ApplicationCreateInfo);
        virtual ~CApplication();

        void Init(SApplicationCreateInfo const &ApplicationCreateInfo);
        void Run();

        void                       PushLayer(TOwn<CLayer> &&NewLayer);
        [[nodiscard]] TOwn<CLayer> PopLayer();

        void UpdateLayers(FTimeDelta ElapsedTime);
        void RenderLayers(FTimeDelta ElapsedTime);
        void OnEventReceived(CEvent &Event);

        CWindow &GetWindow() { return *m_Window; };

        CGUIController &GetGUIController() { return m_GUIController; };

        static CApplication &GetInstance() { return *s_ApplicationInstance; }

    private:
        void InitWindow(SWindowInitInfo const &WindowInitInfo);
        void InitGUIController();
        void InitRenderer();

        TOwn<CWindow> m_Window;

        CGUIController m_GUIController;

        CLayersStack m_LayersStack;

        static CApplication *s_ApplicationInstance;
    };

} // namespace Corvus

#endif //! CORVUS_SOURCE_CORVUS_CORE_APPLICATION_H