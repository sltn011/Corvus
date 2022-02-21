#ifndef CORVUS_SOURCE_CORVUS_CORE_ENTRYPOINT_H
#define CORVUS_SOURCE_CORVUS_CORE_ENTRYPOINT_H

#include "Base.h"
#include "Application.h"

#ifdef CORVUS_PLATFORM_WINDOWS

    extern Corvus::Application *Corvus::CreateApplication();
    extern bool Corvus::DestroyApplication(Corvus::Application *App);
    
    int main(int argc, char **argv)
    {
        Corvus::Log::Init();

        CORVUS_CORE_TRACE("Trace");
        CORVUS_CORE_INFO("Info");
        CORVUS_CORE_WARN("Warn");
        CORVUS_CORE_ERROR("Error");
        CORVUS_CORE_CRITICAL("Critical");

        CORVUS_TRACE("Trace");
        CORVUS_INFO("Info");
        CORVUS_WARN("Warn");
        CORVUS_ERROR("Error");
        CORVUS_CRITICAL("Critical");

        Corvus::Application *App = Corvus::CreateApplication();
        App->Run();
        DestroyApplication(App);
    }

#endif

#endif //!CORVUS_SOURCE_CORVUS_CORE_ENTRYPOINT_H