#include "CorvusPCH.h"
#include "Corvus/Core/Application.h"

namespace Corvus 
{

    Application::Application()
    {

    }

    Application::~Application()
    {

    }

    void Application::Run()
    {
        CORVUS_CORE_TRACE(STR("Running the application!"));

        while (1);

        CORVUS_CORE_TRACE(STR("Application finished running!"));
    }

}