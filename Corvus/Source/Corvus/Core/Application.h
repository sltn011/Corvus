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
    bool DestroyApplication(Application *App);
    //=========================================

    class TimeDelta;

    class Application 
    {
    public:

        Application();
        virtual ~Application();

        void Init();
        void Run();

        void PushLayer(Own<Layer> &&NewLayer);
        Own<Layer> PopLayer();

        void UpdateLayers(TimeDelta ElapsedTime);
        void RenderLayers();
        void OnEventReceived(Event &Event);

        Window &GetWindow();

        static Application &GetInstance() { return *s_ApplicationInstance; }

    protected:

        void InitWindow();
        void InitRenderer();

        Own<Window>   m_Window;
        LayersStack   m_LayersStack;

    private:

        static Application *s_ApplicationInstance;
    };

}

#endif //!CORVUS_SOURCE_CORVUS_CORE_APPLICATION_H