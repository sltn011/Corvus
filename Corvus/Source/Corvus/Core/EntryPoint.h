#ifndef CORVUS_SOURCE_CORVUS_CORE_ENTRYPOINT_H
#define CORVUS_SOURCE_CORVUS_CORE_ENTRYPOINT_H

#include "Base.h"
#include "Application.h"

#ifdef CORVUS_PLATFORM_WINDOWS

    extern Corvus::Application *Corvus::CreateApplication();
    extern bool Corvus::DestroyApplication(Corvus::Application *App);
    
    int main(int argc, char **argv)
    {
        Corvus::Application *App = Corvus::CreateApplication();
        App->Run();
        DestroyApplication(App);
    }

#endif

#endif //!CORVUS_SOURCE_CORVUS_CORE_ENTRYPOINT_H