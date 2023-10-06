#ifndef CORVUS_SOURCE_CORVUS_CORE_APPLICATION_H
#define CORVUS_SOURCE_CORVUS_CORE_APPLICATION_H

#include "Corvus/Core/Base.h"
#include "Corvus/Core/LayersStack.h"
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

        void UpdateLayers(FTimeDelta ElapsedTime);
        void RenderLayers();
        void OnEventReceived(CEvent &Event);

        CWindow &GetWindow();

        static CApplication &GetInstance() { return *s_ApplicationInstance; }

        CScene Scene;

    private:
        void InitWindow();
        void InitRenderer();
        void DestroyRenderer();

        TOwn<CWindow> m_Window;
        CLayersStack  m_LayersStack;

        static CApplication *s_ApplicationInstance;
    };

} // namespace Corvus

#endif //! CORVUS_SOURCE_CORVUS_CORE_APPLICATION_H