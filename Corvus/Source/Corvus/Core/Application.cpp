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
    }

    Application::~Application()
    {

    }

    void Application::Run()
    {
        CORVUS_CORE_INFO("Running the application!");

        // Triangle Data
        GLuint VAO, VBO, EBO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        float vertices[] = {
            -0.2f, -0.2f, 0.0f,
            +0.2f, -0.2f, 0.0f,
            +0.0f, +0.5f, 0.0f
        };

        int indices[] = {
            0, 1, 2
        };

        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);

        // Shader
        GLuint Shader = glCreateProgram();
        GLchar ShaderLog[512];
        GLint Status;

        String VertexCode =
            "#version 460 core\n"
            "\n"
            "layout(location = 0) in vec4 inPos;\n"
            "\n"
            "void main()\n"
            "{\n"
            "    gl_Position = inPos;\n"
            "}\n";

        GLuint VertexShader = glCreateShader(GL_VERTEX_SHADER);
        GLchar const *VertexShaderCode = VertexCode.c_str();
        glShaderSource(VertexShader, 1, &VertexShaderCode, nullptr);
        glCompileShader(VertexShader);
        glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &Status);
        if (!Status)
        {
            glGetShaderInfoLog(VertexShader, 512, nullptr, ShaderLog);
            CORVUS_CORE_ERROR("{:s}", ShaderLog);
        }

        String FragmentCode =
            "#version 460 core\n"
            "\n"
            "out vec4 outColor;\n"
            "\n"
            "void main()\n"
            "{\n"
            "    outColor = vec4(0.1f, 0.5f, 0.1f, 1.0f);\n"
            "}\n";

        GLuint FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        GLchar const *FragmentShaderCode = FragmentCode.c_str();
        glShaderSource(FragmentShader, 1, &FragmentShaderCode, nullptr);
        glCompileShader(FragmentShader);
        glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &Status);
        if (!Status)
        {
            glGetShaderInfoLog(FragmentShader, 512, nullptr, ShaderLog);
            CORVUS_CORE_ERROR("{:s}", ShaderLog);
        }

        glAttachShader(Shader, VertexShader);
        glAttachShader(Shader, FragmentShader);
        glLinkProgram(Shader);
        glGetProgramiv(Shader, GL_LINK_STATUS, &Status);
        if (!Status)
        {
            glGetProgramInfoLog(Shader, 512, nullptr, ShaderLog);
            CORVUS_CORE_ERROR("{:s}", ShaderLog);
        }

        glDeleteShader(VertexShader);
        glDeleteShader(FragmentShader);


        while (!glfwWindowShouldClose(static_cast<GLFWwindow *>(m_Window->GetRawWindow()))) {

            glClearColor(0.6f, 0.8f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            glUseProgram(Shader);
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);

            UpdateLayers();
            RenderLayers();

            m_Window->OnUpdate();
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