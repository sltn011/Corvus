#include "CorvusPCH.h"
#include "Corvus/Core/Application.h"

#include "Corvus/Renderer/Renderer.h"
#include "Corvus/Renderer/VertexArray.h"
#include "Corvus/Renderer/VertexBuffer.h"
#include "Corvus/Renderer/IndexBuffer.h"
#include "Corvus/Renderer/Shader.h"

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
        Renderer::Init();
    }

    Application::~Application()
    {

    }

    void Application::Run()
    {
        CORVUS_CORE_INFO("Running the application!");

        float Vertices[] = {
            -0.2f, -0.2f, 0.0f, 1.0f, 0.0f, 0.0f,
            +0.2f, -0.2f, 0.0f, 0.0f, 1.0f, 0.0f,
            +0.0f, +0.5f, 0.0f, 0.0f, 0.0f, 1.0f
        };

        UInt32 Indices[] = {
            0, 1, 2
        };

        VertexBufferLayout Layout = {
            { BufferDataType::Vec3 },
            { BufferDataType::Vec3 }
        };

        Own<VertexBuffer> VBO = VertexBuffer::Create(Vertices, 3, Layout);
        Own<IndexBuffer> EBO = IndexBuffer::Create(Indices, 3);

        Own<VertexArray> VAO = VertexArray::Create();
        VAO->AddVertexBuffer(std::move(VBO));
        VAO->AddIndexBuffer(std::move(EBO));

        Own<Shader> TestShader = Shader::CreateFromFile("./Assets/Shaders/TestShader.glsl");

        while (!glfwWindowShouldClose(static_cast<GLFWwindow *>(m_Window->GetRawWindow()))) {

            Renderer::BeginScene();

            Renderer::SetClearColor({ 0.6f, 0.8f, 1.0f, 1.0f });
            Renderer::Clear();

            Renderer::Submit(VAO, TestShader);

            Renderer::EndScene();

            UpdateLayers();
            RenderLayers();

            m_Window->OnUpdate();
        }

        CORVUS_CORE_INFO("Application finished running!");
    }

    void Application::PushLayer(Own<Layer> NewLayer)
    {
        m_LayersStack.PushLayer(std::move(NewLayer));
    }

    Own<Layer> Application::PopLayer()
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
        if (!m_Window->GetGUIController().IsInitialized())
        {
            return;
        }

        m_Window->GetGUIController().BeginFrame();
        for (auto It = m_LayersStack.Begin(); It != m_LayersStack.End(); ++It)
        {
            (*It)->Render();
        }
        m_Window->GetGUIController().EndFrame();
    }

    void Application::OnEventReceived(Event &Event)
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
        return *m_Window;
    }

    void Application::InitWindow()
    {
        WindowData WindowSettings;
        WindowSettings.WindowWidth = 1280;
        WindowSettings.WindowHeight = 720;
        WindowSettings.WindowName = "TestWindow";
        WindowSettings.bVSyncEnabled = true;

        m_Window = Window::Create();
        CORVUS_CORE_ASSERT(m_Window);
        m_Window->Init(WindowSettings);
        m_Window->OnEvent.BindObject(this, &Application::OnEventReceived);

        CORVUS_CORE_INFO("Application Window \"{0}\" {1}x{2} initialized", 
            m_Window->GetWindowName(), m_Window->GetWindowWidth(), m_Window->GetWindowHeight());
    }
}