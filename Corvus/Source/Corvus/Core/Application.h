#ifndef CORVUS_SOURCE_CORVUS_CORE_APPLICATION_H
#define CORVUS_SOURCE_CORVUS_CORE_APPLICATION_H

#include "Corvus/Core/Base.h"
#include "Corvus/Core/LayersStack.h"
#include "Corvus/Window/Window.h"

namespace Corvus
{
    class Application;

    // To be defined by Client
    Application *CreateApplication();
    bool         DestroyApplication(Application *App);
    //=========================================

    class TimeDelta;

    class Application
    {
    public:
        Application();
        virtual ~Application();

        void Init();
        void Run();

        void                      PushLayer(TOwn<Layer> &&NewLayer);
        [[nodiscard]] TOwn<Layer> PopLayer();

        void UpdateLayers(TimeDelta ElapsedTime);
        void RenderLayers();
        void OnEventReceived(Event &Event);

        Window &GetWindow();

        static Application &GetInstance() { return *s_ApplicationInstance; }

    private:
        void InitWindow();
        void InitRenderer();

        TOwn<Window> m_Window;
        LayersStack  m_LayersStack;

        static Application *s_ApplicationInstance;
    };

} // namespace Corvus

#endif //! CORVUS_SOURCE_CORVUS_CORE_APPLICATION_H