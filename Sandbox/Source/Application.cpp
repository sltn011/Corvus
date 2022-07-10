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
                +0.0f, -0.2f, -0.2f, 1.0f, 0.0f, 0.0f,
                +0.0f, -0.2f, +0.2f, 0.0f, 1.0f, 0.0f,
                +0.0f, +0.2f, +0.2f, 0.0f, 0.0f, 1.0f,
                +0.0f, +0.2f, -0.2f, 1.0f, 1.0f, 0.0f
            };

            UInt32 Indices[] = {
                0, 1, 2, 0, 2, 3
            };

            VertexBufferLayout Layout = {
                { BufferDataType::Vec3 },
                { BufferDataType::Vec3 }
            };

            Own<VertexBuffer> VBO = VertexBuffer::Create(Vertices, 4, Layout);
            Own<IndexBuffer> EBO = IndexBuffer::Create(Indices, 6);

            VAO = VertexArray::Create();
            VAO->AddVertexBuffer(std::move(VBO));
            VAO->AddIndexBuffer(std::move(EBO));

            TestShader = Shader::CreateFromFile("./Assets/Shaders/TestShader.glsl");

            UInt32 WindowWidth  = Application::GetInstance().GetWindow().GetWindowWidth();
            UInt32 WindowHeight = Application::GetInstance().GetWindow().GetWindowHeight();
            Camera.SetViewportSize(WindowWidth, WindowHeight);
            Camera.SetFoVAngle(60.0f);
            Camera.SetClipPlanes(0.01f, 100.0f);
            Camera.SwitchPlayerControl(true, 1.0f);

            ObjectTransform.SetWorldPosition(glm::vec3(1.0f, 0.0f, 0.0f));
            Rotation ObjectRotation = ObjectTransform.GetRotation();
            ObjectRotation.SetRotationOrder(RotationOrder::YXZ);
            ObjectRotation.SetRollAngle(30.0f);
            ObjectTransform.SetRotation(ObjectRotation);
        }

        virtual void OnUpdate(TimeDelta ElapsedTime)
        {
            Renderer::BeginScene();

            Renderer::SetClearColor({ 0.6f, 0.8f, 1.0f, 1.0f });
            Renderer::Clear();

            if (bCameraMode)
            {
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

            }

            glm::vec2 NewPos = Input::GetCursorPos();
            glm::vec2 Delta = NewPos - CursorPos;
            CursorPos = NewPos;
            if (bCameraMode)
            {
                Camera.ProcessRotationInput(Delta.x, Delta.y, 10.0f, ElapsedTime);
            }

            Rotation Rotator = ObjectTransform.GetRotation();
            Rotator.AddYawAngle(20.0f * ElapsedTime.Seconds());
            ObjectTransform.SetRotation(Rotator);

            TestShader->Bind();
            TestShader->SetMat4("u_Transform", ObjectTransform.GetTransformMatrix());
            TestShader->SetMat4("u_ProjView", Camera.GetProjectionViewMatrix());
            Renderer::Submit(VAO, TestShader);

            Renderer::EndScene();
        }

        virtual void OnEvent(Event &Event)
        {
            if (Event.GetEventType() == Event::EType::KeyPress)
            {
                KeyPressEvent &KPEvent = CastEvent<KeyPressEvent>(Event);
                if (KPEvent.Key == Key::C)
                {
                    bCameraMode = !bCameraMode;
                    Input::SetCursorEnabled(!bCameraMode);

                    Event.SetHandled();
                }
            }
            else if (Event.GetEventType() == Event::EType::WindowResize)
            {
                WindowResizeEvent &WREvent = CastEvent<WindowResizeEvent>(Event);
                Camera.SetViewportSize(WREvent.NewWidth, WREvent.NewHeight);
            }
        }

    protected:

        Own<VertexArray>  VAO;
        Own<Shader>       TestShader;
        PerspectiveCamera Camera;

        bool bCameraMode = false;
        glm::vec2 CursorPos;

        Transform ObjectTransform;
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