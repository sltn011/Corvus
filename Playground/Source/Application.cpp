#include <Corvus.h>

#include "Corvus/Components/BaseSceneComponent.h"

namespace Corvus {
    
    class Playground : public Application
    {
    public:

        Playground() {}
        ~Playground() {}

    };

    class ApplicationLayer : public Layer
    {
    public:

        ApplicationLayer()
            : Layer{ "ApplicationLayer", true }
        {
            Renderer::EnableDepthTest();
            Renderer::SetClearColor({ 0.6f, 0.8f, 1.0f, 1.0f });

            float const Vertices[] = {
                +0.0f, -0.2f, -0.2f, 1.0f, 0.0f, 0.0f,
                +0.0f, -0.2f, +0.2f, 0.0f, 1.0f, 0.0f,
                +0.0f, +0.2f, +0.2f, 0.0f, 0.0f, 1.0f,
                +0.0f, +0.2f, -0.2f, 1.0f, 1.0f, 0.0f
            };

            UInt32 const Indices[] = {
                0, 1, 2, 0, 2, 3
            };

            VertexBufferLayout const Layout = {
                { BufferDataType::Vec3 },
                { BufferDataType::Vec3 }
            };

            Own<VertexBuffer> VBO = VertexBuffer::Create(Vertices, 4, Layout);
            Own<IndexBuffer>  EBO = IndexBuffer::Create(Indices, 6);

            VAO = VertexArray::Create();
            VAO->AddVertexBuffer(std::move(VBO));
            VAO->AddIndexBuffer(std::move(EBO));

            TestShader = Shader::CreateFromFile("./Assets/Shaders/TestShader.glsl");

            UInt32 const WindowWidth  = Application::GetInstance().GetWindow().GetWindowWidth();
            UInt32 const WindowHeight = Application::GetInstance().GetWindow().GetWindowHeight();
            Camera.SetViewportSize(static_cast<float>(WindowWidth), static_cast<float>(WindowHeight));
            Camera.SetFoVAngle(60.0f);
            Camera.SetClipPlanes(0.01f, 100.0f);
            Camera.SwitchPlayerControl(true, 1.0f);

            Entities.EmplaceBack(
                TestShader,
                VAO, 
                Transform{ {1.0f, 0.0f, 0.0f}, Vector::OneVec, {RotationOrder::YXZ, {30.0f, 0.0f, 0.0f}} }
            );

            Entities.EmplaceBack(
                TestShader,
                VAO,
                Transform{ {0.0f, 0.0f, 0.5f}, Vector::OneVec * 0.5f, {RotationOrder::YXZ, {0.0f, 0.0f, 45.0f}} }
            );

            Entities[0].TransformComponent->AddChild(Entities[1].TransformComponent.Get());

        }

        virtual void OnUpdate(TimeDelta ElapsedTime)
        {
            Renderer::BeginScene();
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

            Vec2 const NewPos = Input::GetCursorPos();
            Vec2 const Delta = NewPos - CursorPos;
            CursorPos = NewPos;
            if (bCameraMode)
            {
                Camera.ProcessRotationInput(Delta.x, Delta.y, 10.0f, ElapsedTime);
            }

            for (SizeT i = 0; i < Entities.GetSize(); ++i)
            {
                Entity &SceneEntity = Entities[i];

                Transform EntityTransform = SceneEntity.TransformComponent->GetTransform();
                Rotation Rotator = EntityTransform.GetRotation();
                Rotator.AddYawDegrees(-20.0f * ElapsedTime.Seconds());
                EntityTransform.SetRotation(Rotator);
                SceneEntity.TransformComponent.Get()->SetTransform(EntityTransform);

                TestShader->Bind();
                TestShader->SetMat4("u_Transform", SceneEntity.TransformComponent->GetTransformMatrix());
                TestShader->SetMat4("u_ProjView", Camera.GetProjectionViewMatrix());
                Renderer::Submit(VAO, TestShader);
            }

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
                Camera.SetViewportSize(static_cast<float>(WREvent.NewWidth), static_cast<float>(WREvent.NewHeight));
            }
        }

    protected:

        Array<Entity> Entities;
        PerspectiveCamera   Camera;

        Own<Shader>      TestShader;
        Own<VertexArray> VAO;

        bool bCameraMode = false;
        Vec2 CursorPos;
    };

    Application *CreateApplication()
    {
        Playground *App = new Playground;
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