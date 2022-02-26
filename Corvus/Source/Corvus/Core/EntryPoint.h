#ifndef CORVUS_SOURCE_CORVUS_CORE_ENTRYPOINT_H
#define CORVUS_SOURCE_CORVUS_CORE_ENTRYPOINT_H

#include "Corvus/Core/Base.h"
#include "Corvus/Core/Application.h"

#ifdef CORVUS_PLATFORM_WINDOWS

    extern Corvus::Application *Corvus::CreateApplication();
    extern bool Corvus::DestroyApplication(Corvus::Application *App);
    
    int main(int argc, char **argv)
    {
        Corvus::Log::Init();

        Corvus::Application *App = Corvus::CreateApplication();
        if (App)
        {
            CORVUS_CORE_TRACE("Application successfully created!");
            CORVUS_CORE_TRACE("Running the application!");

            App->Run();

            CORVUS_CORE_TRACE("Application finished running!");

            if (DestroyApplication(App)) {
                CORVUS_CORE_TRACE("Application resources cleaned up successfully on exit!");
            }
            else {
                CORVUS_CORE_ERROR("Error cleaning up application resources on exit!");
            }
        }
    }

#endif

#endif //!CORVUS_SOURCE_CORVUS_CORE_ENTRYPOINT_H