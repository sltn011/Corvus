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

        void PushLayer(Own<Layer> NewLayer);
        Own<Layer> PopLayer();

        void UpdateLayers();
        void RenderLayers();
        void OnEventReceived(Event &Event);

        Window &GetWindow();

        static Application &GetInstance() { return *s_ApplicationInstance; }

    protected:

        void InitWindow();

        Own<Window>   m_Window;
        LayersStack   m_LayersStack;

    private:

        static Application *s_ApplicationInstance;
    };

    // To be defined by Client
    Application *CreateApplication();
    bool DestroyApplication(Application *App);

}

#endif //!CORVUS_SOURCE_CORVUS_CORE_APPLICATION_H