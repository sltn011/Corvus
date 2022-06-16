#include <Corvus.h>

class Sandbox : public Corvus::Application 
{
public:
    
    Sandbox(){}
    ~Sandbox() {}

};

class ApplicationLayer : public Corvus::Layer
{
public:

    ApplicationLayer() 
        : Corvus::Layer{ "ApplicationLayer", true }
    {
        float Vertices[] = {
            +1.0f, -0.2f, -0.2f, 1.0f, 0.0f, 0.0f,
            +1.0f, -0.2f, +0.2f, 0.0f, 1.0f, 0.0f,
            +1.0f, +0.5f, +0.0f, 0.0f, 0.0f, 1.0f
        };

        UInt32 Indices[] = {
            0, 1, 2
        };

        Corvus::VertexBufferLayout Layout = {
            { Corvus::BufferDataType::Vec3 },
            { Corvus::BufferDataType::Vec3 }
        };

        Corvus::Own<Corvus::VertexBuffer> VBO = Corvus::VertexBuffer::Create(Vertices, 3, Layout);
        Corvus::Own<Corvus::IndexBuffer> EBO = Corvus::IndexBuffer::Create(Indices, 3);

        VAO = Corvus::VertexArray::Create();
        VAO->AddVertexBuffer(std::move(VBO));
        VAO->AddIndexBuffer(std::move(EBO));

        TestShader = Corvus::Shader::CreateFromFile("./Assets/Shaders/TestShader.glsl");

        Camera.SwitchPlayerControl(true, 1.0f);
    }

    virtual ~ApplicationLayer() {}

    virtual void OnUpdate(Corvus::TimeDelta ElapsedTime)
    {
        Corvus::Renderer::BeginScene();

        Corvus::Renderer::SetClearColor({ 0.6f, 0.8f, 1.0f, 1.0f });
        Corvus::Renderer::Clear();

        if (Corvus::Input::IsKeyPressed(Corvus::Key::W))
        {
            Camera.ProcessMovementInput(Corvus::Camera::MoveDirection::Forward, ElapsedTime);
        }
        if (Corvus::Input::IsKeyPressed(Corvus::Key::A))
        {
            Camera.ProcessMovementInput(Corvus::Camera::MoveDirection::Left, ElapsedTime);
        }
        if (Corvus::Input::IsKeyPressed(Corvus::Key::S))
        {
            Camera.ProcessMovementInput(Corvus::Camera::MoveDirection::Backward, ElapsedTime);
        }
        if (Corvus::Input::IsKeyPressed(Corvus::Key::D))
        {
            Camera.ProcessMovementInput(Corvus::Camera::MoveDirection::Right, ElapsedTime);
        }
        if (Corvus::Input::IsKeyPressed(Corvus::Key::Space))
        {
            Camera.ProcessMovementInput(Corvus::Camera::MoveDirection::Up, ElapsedTime);
        }
        if (Corvus::Input::IsKeyPressed(Corvus::Key::LeftShift))
        {
            Camera.ProcessMovementInput(Corvus::Camera::MoveDirection::Down, ElapsedTime);
        }

        TestShader->Bind();
        TestShader->SetMat4("u_ProjView", Camera.GetProjectionViewMatrix());
        Corvus::Renderer::Submit(VAO, TestShader);

        Corvus::Renderer::EndScene();
    }

    virtual void OnEvent(Corvus::Event &Event)
    {
        
    }

protected:

    Corvus::Own<Corvus::VertexArray> VAO;
    Corvus::Own<Corvus::Shader>      TestShader;
    Corvus::PerspectiveCamera        Camera;

};

namespace Corvus {
    
    Application *CreateApplication()
    {
        Sandbox *App = new Sandbox;
        App->PushLayer(Corvus::MakeOwned<ApplicationLayer>());
        return App;
    }

    bool DestroyApplication(Application *App)
    {
        if (!App) {
            return false;
        }

        delete App;
        return true;
    }

}