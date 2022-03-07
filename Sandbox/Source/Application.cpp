#include <Corvus.h>

#include <Corvus/Debug/LayerDebug.h>

class Sandbox : public Corvus::Application 
{
public:
    
    Sandbox(){}
    ~Sandbox() {}

};

namespace Corvus {
    
    Application *CreateApplication()
    {
        Sandbox *App = new Sandbox;
        App->PushLayer(MakeOwned<LayerDebug>("Debug Layer", true));
        return App;
    }

    bool DestroyApplication(Application *App)
    {
        if (!App) {
            return false;
        }

        delete App;
        return true;
    }

}