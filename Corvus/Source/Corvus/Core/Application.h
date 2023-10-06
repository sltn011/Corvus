#ifndef CORVUS_SOURCE_CORVUS_CORE_APPLICATION_H
#define CORVUS_SOURCE_CORVUS_CORE_APPLICATION_H

#include "Corvus/Core/Base.h"
#include "Corvus/Core/LayersStack.h"
#include "Corvus/GUI/LayerGUI.h"
#include "Corvus/Scene/Scene.h"
#include "Corvus/Window/Window.h"

namespace Corvus
{
    class CApplication;

    // To be defined by Client
    CApplication *CreateApplication();
    bool          DestroyApplication(CApplication *App);
    //=========================================

    class FTimeDelta;

    class CApplication
    {
    public:
        CApplication();
        virtual ~CApplication();

        void Init();
        void Run();

        void                       PushLayer(TOwn<CLayer> &&NewLayer);
        [[nodiscard]] TOwn<CLayer> PopLayer();

        void                          PushGUILayer(TOwn<CLayerGUI> &&NewLayer);
        [[nodiscard]] TOwn<CLayerGUI> PopGUILayer();

        void UpdateLayers(FTimeDelta ElapsedTime);
        void RenderGUILayers();
        void OnEventReceived(CEvent &Event);

        CWindow &GetWindow();

        static CApplication &GetInstance() { return *s_ApplicationInstance; }

        CScene Scene;

    private:
        void InitWindow();
        void InitRenderer();
        void DestroyRenderer();

        TOwn<CWindow>           m_Window;
        CLayersStack<CLayer>    m_LayersStack;
        CLayersStack<CLayerGUI> m_GUILayersStack;

        static CApplication *s_ApplicationInstance;
    };

} // namespace Corvus

#endif //! CORVUS_SOURCE_CORVUS_CORE_APPLICATION_H