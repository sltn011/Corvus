#include "CorvusPCH.h"
#include "Corvus/Core/Application.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Corvus 
{

    Application::Application()
    {
        InitWindow();
        InitRenderingContext();
    }

    Application::~Application()
    {

    }

    void Application::Run()
    {
        CORVUS_CORE_INFO("Running the application!");

        while (!glfwWindowShouldClose(m_ApplicationWindow.GetWindow())) {

            glClearColor(0.3f, 0.15f, 0.6f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            // Swap front and back buffers
            glfwSwapBuffers(m_ApplicationWindow.GetWindow());

            // Process pending events
            glfwPollEvents();
        }

        CORVUS_CORE_INFO("Application finished running!");
    }

    void Application::PushLayer(Own<LayerBase> NewLayer)
    {
        m_LayersStack.PushLayer(std::move(NewLayer));
    }

    Own<LayerBase> Application::PopLayer()
    {
        return m_LayersStack.PopLayer();
    }

    void Application::OnUpdate()
    {
        for (auto It = m_LayersStack.Begin(); It != m_LayersStack.End(); ++It)
        {
            (*It)->OnUpdate();
        }
    }

    void Application::OnEventReceived(EventBase &Event)
    {
        for (auto It = m_LayersStack.RBegin(); It != m_LayersStack.REnd(); ++It) 
        {
            (*It)->OnEvent(Event);
            if (Event.WasHandled()) {
                return;
            }
        }
    }

    void Application::InitWindow()
    {
        WindowData WindowSettings;
        WindowSettings.WindowWidth = 500;
        WindowSettings.WindowHeight = 400;
        WindowSettings.WindowName = "TestWindow";

        m_ApplicationWindow.Init(WindowSettings);
        m_ApplicationWindow.OnEvent.BindObject(this, &Application::OnEventReceived);

        CORVUS_CORE_INFO("Application Window \"{0}\" initialized", m_ApplicationWindow.GetWindowName());
    }

    void Application::InitRenderingContext()
    {
        glfwMakeContextCurrent(m_ApplicationWindow.GetWindow());

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            CORVUS_NO_ENTRY_FMT("Failed to initialize rendering context!");
        }

        glViewport(0, 0, m_ApplicationWindow.GetWindowWidth(), m_ApplicationWindow.GetWindowHeight());

        CORVUS_CORE_INFO("Rendering context initialized for Window \"{0}\"", m_ApplicationWindow.GetWindowName());
    }

}