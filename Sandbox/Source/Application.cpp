#include <Corvus.h>

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