#ifndef CORVUS_SOURCE_CORVUS_CORE_APPLICATION_H
#define CORVUS_SOURCE_CORVUS_CORE_APPLICATION_H

#include "Corvus/Core/Base.h"
#include "Corvus/Core/LayersStack.h"
#include "Corvus/GUI/GUIController.h"
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

        void UpdateLayers();
        void RenderLayers();
        void OnEventReceived(EventBase &Event);

        Window &GetWindow();

        static Application &GetInstance() { return *s_ApplicationInstance; }

    protected:

        void InitWindow();
        void InitRenderingContext();
        void InitGUIRenderingContext();

        Own<Window>   m_Window;
        LayersStack   m_LayersStack;
        GUIController m_GUIController;

    private:

        static Application *s_ApplicationInstance;
    };

    // To be defined by Client
    Application *CreateApplication();
    bool DestroyApplication(Application *App);

}

#endif //!CORVUS_SOURCE_CORVUS_CORE_APPLICATION_H