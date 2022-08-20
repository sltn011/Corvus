#ifndef CORVUS_SOURCE_CORVUS_CORE_ENTRYPOINT_H
#define CORVUS_SOURCE_CORVUS_CORE_ENTRYPOINT_H

#include "Corvus/Core/Application.h"
#include "Corvus/Core/Base.h"
#include "Corvus/GUI/LayerGUI.h"

#ifdef CORVUS_PLATFORM_WINDOWS

extern Corvus::Application *Corvus::CreateApplication();
extern bool                 Corvus::DestroyApplication(Corvus::Application *App);

int main(int argc, char **argv)
{
    Corvus::Log::Init();

    Corvus::Application *App = Corvus::CreateApplication();
    if (!App)
    {
        CORVUS_CORE_NO_ENTRY_FMT("Application was not created properly!");
    }
    CORVUS_CORE_INFO("Application successfully created and initialized!");

    CORVUS_CORE_INFO("Running the application!");
    App->Run();
    CORVUS_CORE_INFO("Application finished running!");

    if (DestroyApplication(App))
    {
        CORVUS_CORE_TRACE("Application resources cleaned up successfully on exit!");
    }
    else
    {
        CORVUS_CORE_ERROR("Error cleaning up application resources on exit!");
    }

    return 0;
}

#endif

#endif //! CORVUS_SOURCE_CORVUS_CORE_ENTRYPOINT_H