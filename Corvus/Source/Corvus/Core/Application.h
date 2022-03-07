#ifndef CORVUS_SOURCE_CORVUS_CORE_APPLICATION_H
#define CORVUS_SOURCE_CORVUS_CORE_APPLICATION_H

#include "Corvus/Core/Base.h"
#include "Corvus/Core/LayersStack.h"
#include "Corvus/Window/Window.h"

namespace Corvus 
{

    class Application 
    {
    public:

        Application();
        virtual ~Application();

        void Run();

        void PushLayer(Own<LayerBase> NewLayer);
        Own<LayerBase> PopLayer();

        void OnUpdate();
        void OnEventReceived(EventBase &Event);

    protected:

        void InitWindow();
        void InitRenderingContext();

        Window      m_ApplicationWindow;
        LayersStack m_LayersStack;
    };

    // To be defined by Client
    Application *CreateApplication();
    bool DestroyApplication(Application *App);

}

#endif //!CORVUS_SOURCE_CORVUS_CORE_APPLICATION_H