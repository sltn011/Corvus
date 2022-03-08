#ifndef CORVUS_SOURCE_CORVUS_CORE_ENTRYPOINT_H
#define CORVUS_SOURCE_CORVUS_CORE_ENTRYPOINT_H

#include "Corvus/Core/Base.h"
#include "Corvus/Core/Application.h"

#include "Corvus/GUI/LayerGUI.h"

#ifdef CORVUS_PLATFORM_WINDOWS

    extern Corvus::Application *Corvus::CreateApplication();
    extern bool Corvus::DestroyApplication(Corvus::Application *App);
    
    int main(int argc, char **argv)
    {
        Corvus::Log::Init();

        Corvus::Application *App = Corvus::CreateApplication();
        if (App)
        {
            Corvus::Own<Corvus::LayerGUI> GUILayer = Corvus::MakeOwned<Corvus::LayerGUI>("GUI", true);
            App->PushLayer(std::move(GUILayer));

            CORVUS_CORE_TRACE("Application successfully created!");

            App->Run();

            if (DestroyApplication(App)) 
            {
                CORVUS_CORE_TRACE("Application resources cleaned up successfully on exit!");
            }
            else 
            {
                CORVUS_CORE_ERROR("Error cleaning up application resources on exit!");
            }
        }
        else
        {
            CORVUS_CORE_CRITICAL("Application was not created properly!");
        }
    }

#endif

#endif //!CORVUS_SOURCE_CORVUS_CORE_ENTRYPOINT_H