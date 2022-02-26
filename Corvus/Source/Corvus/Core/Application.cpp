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
        CORVUS_CORE_TRACE("Running the application!");

        while (1);

        CORVUS_CORE_TRACE("Application finished running!");
    }

}