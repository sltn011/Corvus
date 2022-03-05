#include "CorvusPCH.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
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

        WindowData WindowSettings;
        WindowSettings.WindowWidth = 500;
        WindowSettings.WindowHeight = 400;
        WindowSettings.WindowName = "TestWindow";

        ApplicationWindow.Init(WindowSettings);
        ApplicationWindow.OnEvent.BindObject(this, &Application::OnEventReceived);

        glfwMakeContextCurrent(ApplicationWindow.GetWindow());

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            return;
        }

        glViewport(0, 0, 300, 200);

        while (!glfwWindowShouldClose(ApplicationWindow.GetWindow())) {

            glClearColor(0.3f, 0.15f, 0.6f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            // Swap front and back buffers
            glfwSwapBuffers(ApplicationWindow.GetWindow());

            // Process pending events
            glfwPollEvents();
        }

        CORVUS_CORE_TRACE("Application finished running!");
    }

    void Application::OnEventReceived(EventBase &Event)
    {
        CORVUS_CORE_INFO(Event.ToString());
    }

}