#include <Corvus.h>

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

            Entities.emplace_back(
                TestShader,
                VAO, 
                Transform{ {1.0f, 0.0f, 0.0f}, Vector::OneVec, {RotationOrder::YXZ, {30.0f, 0.0f, 0.0f}} }
            );

            Entities.emplace_back(
                TestShader,
                VAO,
                Transform{ {0.0f, 0.0f, 0.5f}, Vector::OneVec * 0.5f, {RotationOrder::YXZ, {0.0f, 0.0f, 45.0f}} }
            );

            Entities[0].AddChild(&Entities[1]);

            CORVUS_ERROR("Start of memory pooling showcase!");

            // Pool of 2 objects with size 4 bytes and 3 objects with size 10 bytes
            size_t const PoolID1 = AppPools::AddPool({ {2, 4}, {3, 10} });
            CORVUS_TRACE("Created pool with IDs {} and two blocks", PoolID1);
            CORVUS_TRACE("Pool {} for 2 objects with size 4 bytes and 3 objects with size 10 bytes", PoolID1);

            CORVUS_TRACE("3 blocks requested from Pool {} first block (has space for 2 only) and 1 from second block", PoolID1);
            PI1 = AppPools::Request(PoolID1, 0);
            PI2 = AppPools::Request(PoolID1, 0);
            PI3 = AppPools::Request(PoolID1, 0);
            PI4 = AppPools::Request(PoolID1, 1);

            UInt8 *n1 = PI1.GetRaw();
            UInt8 *n2 = PI2.GetRaw();
            UInt8 *n3 = PI3.GetRaw();
            UInt8 *n4 = PI4.GetRaw();
            CORVUS_TRACE("Pointers to pooled objects: {}, {}, {}; {}", (void *)n1, (void *)n2, (void *)n3, (void *)n4);

            PI1.Free();
            n1 = PI1.GetRaw();
            n2 = PI2.GetRaw();
            n3 = PI3.GetRaw();
            n4 = PI4.GetRaw();
            CORVUS_TRACE("After freeing memory in pool index 1:");
            CORVUS_TRACE("Pointers to pooled objects: {}, {}, {}; {}", (void *)n1, (void *)n2, (void *)n3, (void *)n4);

            PI2.Free();
            n1 = PI1.GetRaw();
            n2 = PI2.GetRaw();
            n3 = PI3.GetRaw();
            n4 = PI4.GetRaw();
            CORVUS_TRACE("After freeing memory in pool index 2:");
            CORVUS_TRACE("Pointers to pooled objects: {}, {}, {}; {}", (void *)n1, (void *)n2, (void *)n3, (void *)n4);

            PI3.Free();
            n1 = PI1.GetRaw();
            n2 = PI2.GetRaw();
            n3 = PI3.GetRaw();
            n4 = PI4.GetRaw();
            CORVUS_TRACE("After freeing memory in pool index 3(invalid index):");
            CORVUS_TRACE("Pointers to pooled objects: {}, {}, {}; {}", (void *)n1, (void *)n2, (void *)n3, (void *)n4);

            PI4.Free();
            n1 = PI1.GetRaw();
            n2 = PI2.GetRaw();
            n3 = PI3.GetRaw();
            n4 = PI4.GetRaw();
            CORVUS_TRACE("After freeing memory in pool index 4:");
            CORVUS_TRACE("Pointers to pooled objects: {}, {}, {}; {}", (void *)n1, (void *)n2, (void *)n3, (void *)n4);

            CORVUS_TRACE("Re-requesting 3 blocks from Pool {} first block and one from second block", PoolID1);
            PI1 = AppPools::Request(PoolID1, 0);
            PI2 = AppPools::Request(PoolID1, 0);
            PI3 = AppPools::Request(PoolID1, 0);
            PI4 = AppPools::Request(PoolID1, 1);
            n1 = PI1.GetRaw();
            n2 = PI2.GetRaw();
            n3 = PI3.GetRaw();
            n4 = PI4.GetRaw();
            CORVUS_TRACE("Pointers to pooled objects: {}, {}, {}; {}", (void *)n1, (void *)n2, (void *)n3, (void *)n4);

            CORVUS_ERROR("End of memory pooling showcase!");
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

            Vec2 const NewPos = Input::GetCursorPos();
            Vec2 const Delta = NewPos - CursorPos;
            CursorPos = NewPos;
            if (bCameraMode)
            {
                Camera.ProcessRotationInput(Delta.x, Delta.y, 10.0f, ElapsedTime);
            }

            for (Entity &SceneEntity : Entities)
            {
                Transform EntityTransform = SceneEntity.GetTransform();
                Rotation Rotator = EntityTransform.GetRotation();
                Rotator.AddYawDegrees(-20.0f * ElapsedTime.Seconds());
                EntityTransform.SetRotation(Rotator);
                SceneEntity.SetTransform(EntityTransform);

                TestShader->Bind();
                TestShader->SetMat4("u_Transform", SceneEntity.GetSceneTransformMatrix());
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

        PoolIndex PI1, PI2, PI3, PI4;

        std::vector<Entity> Entities;
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