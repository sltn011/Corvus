#ifndef CORVUS_SOURCE_CORVUS_CORE_APPLICATION_H
#define CORVUS_SOURCE_CORVUS_CORE_APPLICATION_H

#include "Corvus/Core/Base.h"
#include "Corvus/Core/LayersStack.h"
#include "Corvus/Window/Window.h"

namespace Corvus
{
    class СApplication;

    // To be defined by Client
    СApplication *CreateApplication();
    bool          DestroyApplication(СApplication *App);
    //=========================================

    class FTimeDelta;

    class СApplication
    {
    public:
        СApplication();
        virtual ~СApplication();

        void Init();
        void Run();

        void                       PushLayer(TOwn<СLayer> &&NewLayer);
        [[nodiscard]] TOwn<СLayer> PopLayer();

        void UpdateLayers(FTimeDelta ElapsedTime);
        void RenderLayers();
        void OnEventReceived(CEvent &Event);

        CWindow &GetWindow();

        static СApplication &GetInstance() { return *s_ApplicationInstance; }

    private:
        void InitWindow();
        void InitRenderer();

        TOwn<CWindow> m_Window;
        СLayersStack  m_LayersStack;

        static СApplication *s_ApplicationInstance;
    };

} // namespace Corvus

#endif //! CORVUS_SOURCE_CORVUS_CORE_APPLICATION_H