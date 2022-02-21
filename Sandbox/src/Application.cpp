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
        return new Sandbox;
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