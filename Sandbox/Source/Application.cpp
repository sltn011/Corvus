#include <Corvus.h>

namespace Corvus {
    
    class Sandbox : public Application
    {
    public:

        Sandbox() {}
        ~Sandbox() {}

    };

    class ApplicationLayer : public Layer
    {
    public:

        ApplicationLayer()
            : Layer{ "ApplicationLayer", true }
        {
            float Vertices[] = {
                +1.0f, -0.2f, -0.2f, 1.0f, 0.0f, 0.0f,
                +1.0f, -0.2f, +0.2f, 0.0f, 1.0f, 0.0f,
                +1.0f, +0.5f, +0.0f, 0.0f, 0.0f, 1.0f
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

            VAO = VertexArray::Create();
            VAO->AddVertexBuffer(std::move(VBO));
            VAO->AddIndexBuffer(std::move(EBO));

            TestShader = Shader::CreateFromFile("./Assets/Shaders/TestShader.glsl");

            Camera.SwitchPlayerControl(true, 1.0f);
        }

        virtual void OnUpdate(TimeDelta ElapsedTime)
        {
            Renderer::BeginScene();

            Renderer::SetClearColor({ 0.6f, 0.8f, 1.0f, 1.0f });
            Renderer::Clear();

            if (Input::IsKeyPressed(Key::W))
            {
                Camera.ProcessMovementInput(Camera::MoveDirection::Forward, ElapsedTime);
            }
            if (Input::IsKeyPressed(Key::A))
            {
                Camera.ProcessMovementInput(Camera::MoveDirection::Left, ElapsedTime);
            }
            if (Input::IsKeyPressed(Key::S))
            {
                Camera.ProcessMovementInput(Camera::MoveDirection::Backward, ElapsedTime);
            }
            if (Input::IsKeyPressed(Key::D))
            {
                Camera.ProcessMovementInput(Camera::MoveDirection::Right, ElapsedTime);
            }
            if (Input::IsKeyPressed(Key::Space))
            {
                Camera.ProcessMovementInput(Camera::MoveDirection::Up, ElapsedTime);
            }
            if (Input::IsKeyPressed(Key::LeftShift))
            {
                Camera.ProcessMovementInput(Camera::MoveDirection::Down, ElapsedTime);
            }

            TestShader->Bind();
            TestShader->SetMat4("u_ProjView", Camera.GetProjectionViewMatrix());
            Renderer::Submit(VAO, TestShader);

            Renderer::EndScene();
        }

        virtual void OnEvent(Event &Event)
        {

        }

    protected:

        Own<VertexArray>  VAO;
        Own<Shader>       TestShader;
        PerspectiveCamera Camera;

    };

    Application *CreateApplication()
    {
        Sandbox *App = new Sandbox;
        App->PushLayer(Layer::Create<ApplicationLayer>());
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