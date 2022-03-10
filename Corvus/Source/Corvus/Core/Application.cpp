#include "CorvusPCH.h"
#include "Corvus/Core/Application.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Corvus 
{
    Application *Application::s_ApplicationInstance = nullptr;

    Application::Application()
    {
        CORVUS_CORE_ASSERT_FMT(!s_ApplicationInstance, "Only one instance of application is allowed!");

        s_ApplicationInstance = this;

        InitWindow();
        InitRenderingContext();
        InitGUIRenderingContext();
    }

    Application::~Application()
    {

    }

    void Application::Run()
    {
        CORVUS_CORE_INFO("Running the application!");

        while (!glfwWindowShouldClose(static_cast<GLFWwindow *>(m_ApplicationWindow.GetRawWindow()))) {

            glClearColor(0.6f, 0.8f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            UpdateLayers();
            RenderLayers();

            // Swap front and back buffers
            glfwSwapBuffers(static_cast<GLFWwindow *>(m_ApplicationWindow.GetRawWindow()));

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

    void Application::UpdateLayers()
    {
        for (auto It = m_LayersStack.Begin(); It != m_LayersStack.End(); ++It)
        {
            (*It)->OnUpdate();
        }
    }

    void Application::RenderLayers()
    {
        if (!m_GUIController.IsInitialized())
        {
            return;
        }

        m_GUIController.BeginFrame();
        for (auto It = m_LayersStack.Begin(); It != m_LayersStack.End(); ++It)
        {
            (*It)->Render();
        }
        m_GUIController.EndFrame();
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

    Window &Application::GetWindow()
    {
        return m_ApplicationWindow;
    }

    void Application::InitWindow()
    {
        WindowData WindowSettings;
        WindowSettings.WindowWidth = 1280;
        WindowSettings.WindowHeight = 720;
        WindowSettings.WindowName = "TestWindow";

        m_ApplicationWindow.Init(WindowSettings);
        m_ApplicationWindow.OnEvent.BindObject(this, &Application::OnEventReceived);

        CORVUS_CORE_INFO("Application Window \"{0}\" initialized", m_ApplicationWindow.GetWindowName());
    }

    void Application::InitRenderingContext()
    {
        glfwMakeContextCurrent(static_cast<GLFWwindow*>(m_ApplicationWindow.GetRawWindow()));

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            CORVUS_NO_ENTRY_FMT("Failed to initialize rendering context!");
        }

        glViewport(0, 0, m_ApplicationWindow.GetWindowWidth(), m_ApplicationWindow.GetWindowHeight());

        CORVUS_CORE_INFO("Rendering context initialized for Window \"{0}\"", m_ApplicationWindow.GetWindowName());
    }

    void Application::InitGUIRenderingContext()
    {
        m_GUIController.Init();
    }

}