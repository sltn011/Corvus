#include <Corvus.h>

#include "Corvus/Assets/Image/Image.h"
#include "Corvus/Assets/Image/ImageLoader.h"
#include "Corvus/Assets/Material/Material.h"
#include "Corvus/Components/StaticMeshComponent.h"
#include "Corvus/Components/TransformComponent.h"
#include "Corvus/Renderer/Texture2D.h"

namespace Corvus
{

    class CPlayground : public СApplication
    {
    public:
        CPlayground() {}
        ~CPlayground() {}
    };

    class CApplicationLayer : public СLayer
    {
    public:
        CApplicationLayer() : СLayer{"ApplicationLayer", true}
        {
            CRenderer::EnableDepthTest();
            CRenderer::SetClearColor({0.6f, 0.8f, 1.0f, 1.0f});

            InitCamera();
            CreateMeshData();
            PopulateScene();

            CreateShader();
            CreateTexture();
            CreateMaterial();
        }

        virtual void OnUpdate(FTimeDelta const ElapsedTime)
        {
            CRenderer::BeginScene();
            CRenderer::Clear();

            if (bCameraMode)
            {
                if (СInput::IsKeyPressed(Key::W))
                {
                    CCamera.ProcessMovementInput(CCamera::EMoveDirection::Forward, ElapsedTime);
                }
                if (СInput::IsKeyPressed(Key::A))
                {
                    CCamera.ProcessMovementInput(CCamera::EMoveDirection::Left, ElapsedTime);
                }
                if (СInput::IsKeyPressed(Key::S))
                {
                    CCamera.ProcessMovementInput(CCamera::EMoveDirection::Backward, ElapsedTime);
                }
                if (СInput::IsKeyPressed(Key::D))
                {
                    CCamera.ProcessMovementInput(CCamera::EMoveDirection::Right, ElapsedTime);
                }
                if (СInput::IsKeyPressed(Key::Space))
                {
                    CCamera.ProcessMovementInput(CCamera::EMoveDirection::Up, ElapsedTime);
                }
                if (СInput::IsKeyPressed(Key::LeftShift))
                {
                    CCamera.ProcessMovementInput(CCamera::EMoveDirection::Down, ElapsedTime);
                }
            }

            FVector2 const NewPos = СInput::GetCursorPos();
            FVector2 const Delta  = NewPos - CursorPos;
            CursorPos             = NewPos;
            if (bCameraMode)
            {
                CCamera.ProcessRotationInput(Delta.x, Delta.y, 10.0f, ElapsedTime);
            }

            for (SizeT i = 0; i < Entities.GetSize(); ++i)
            {
                CEntity &SceneEntity = Entities[i];

                FTransform EntityTransform = SceneEntity.TransformComponent->GetTransform();
                FRotation  Rotator         = EntityTransform.GetRotation();
                Rotator.AddYawDegrees(-20.0f * ElapsedTime.Seconds());
                EntityTransform.SetRotation(Rotator);
                SceneEntity.TransformComponent.Get()->SetTransform(EntityTransform);

                TestTexture->BindUnit(0);

                TestShader->Bind();
                TestShader->SetMat4("u_Transform", SceneEntity.TransformComponent->GetTransformMatrix());
                TestShader->SetMat4("u_ProjView", CCamera.GetProjectionViewMatrix());

                TestMaterial.LoadInShader(*TestShader);

                CRenderer::Submit(VAO, TestShader);
            }

            CRenderer::EndScene();
        }

        virtual void OnEvent(CEvent &Event)
        {
            if (Event.GetEventType() == CEvent::EEventType::KeyPress)
            {
                СKeyPressEvent &KPEvent = CastEvent<СKeyPressEvent>(Event);
                if (KPEvent.Key == Key::C)
                {
                    bCameraMode = !bCameraMode;
                    СInput::SetCursorEnabled(!bCameraMode);

                    Event.SetHandled();
                }
            }
            else if (Event.GetEventType() == CEvent::EEventType::WindowResize)
            {
                СWindowResizeEvent &WREvent = CastEvent<СWindowResizeEvent>(Event);
                CCamera.SetViewportSize(static_cast<float>(WREvent.NewWidth), static_cast<float>(WREvent.NewHeight));
            }
        }

        void InitCamera()
        {
            UInt32 const WindowWidth  = СApplication::GetInstance().GetWindow().GetWindowWidth();
            UInt32 const WindowHeight = СApplication::GetInstance().GetWindow().GetWindowHeight();
            CCamera.SetViewportSize(static_cast<float>(WindowWidth), static_cast<float>(WindowHeight));
            CCamera.SetFoVAngle(60.0f);
            CCamera.SetClipPlanes(0.01f, 100.0f);
            CCamera.SwitchPlayerControl(true, 1.0f);
        }
        void CreateMeshData()
        {
            // clang-format off
            float const Vertices[] = {
                // Position             // UV
                +0.0f, -0.2f, -0.2f,    0.0f, 0.0f,
                +0.0f, -0.2f, +0.2f,    1.0f, 0.0f,
                +0.0f, +0.2f, +0.2f,    1.0f, 1.0f,
                +0.0f, +0.2f, -0.2f,    0.0f, 1.0f
            };
            // clang-format on

            UInt32 const Indices[] = {0, 1, 2, 0, 2, 3};

            CVertexBufferLayout const Layout = {{EBufferDataType::Vec3}, {EBufferDataType::Vec2}};

            TOwn<CVertexBuffer> VBO = CVertexBuffer::Create(Vertices, 4, Layout);
            TOwn<CIndexBuffer>  EBO = CIndexBuffer::Create(Indices, 6);

            VAO = CVertexArray::Create();
            VAO->AddVertexBuffer(std::move(VBO));
            VAO->AddIndexBuffer(std::move(EBO));
        }
        void PopulateScene()
        {
            Entities.EmplaceBack(
                TestShader,
                VAO,
                FTransform{{1.0f, 0.0f, 0.0f}, FVector::OneVec, {ERotationOrder::YXZ, {30.0f, 0.0f, 0.0f}}}
            );

            Entities.EmplaceBack(
                TestShader,
                VAO,
                FTransform{{0.0f, 0.0f, 0.5f}, FVector::OneVec * 0.5f, {ERotationOrder::YXZ, {0.0f, 0.0f, 45.0f}}}
            );

            Entities[0].TransformComponent->AddChild(Entities[1].TransformComponent.Get());
        }
        void CreateShader() { TestShader = CShader::CreateFromFile("./Assets/Shaders/TestShader.glsl"); }
        void CreateTexture()
        {
            STextureParameters TextureParameters;
            TextureParameters.bHasMipmaps              = true;
            TextureParameters.bHasAnisotropicFiltering = true;
            TextureParameters.MinFiltering             = ETextureFiltering::LinearMipMap_Linear;
            TextureParameters.MagFiltering             = ETextureFiltering::Linear;
            CImage Image =
                CTextureLoader::LoadFromImageFile("./Assets/Textures/OldRabbit.jpg", ELoadTextureChannels::RGB);
            TestTexture = CTexture2D::Create(Image, TextureParameters);
        }
        void CreateMaterial()
        {
            TestMaterial.AlbedoMap.SetTexture(TestTexture.get());
            TestMaterial.AlbedoMap.UseTexture();
        }

    protected:
        TArray<CEntity>    Entities;
        CPerspectiveCamera CCamera;

        TOwn<CShader>      TestShader;
        TOwn<CVertexArray> VAO;

        TOwn<CTexture2D> TestTexture;
        CMaterial        TestMaterial;

        bool     bCameraMode = false;
        FVector2 CursorPos;
    };

    СApplication *CreateApplication()
    {
        CPlayground *App = new CPlayground;
        App->PushLayer(СLayer::Create<CApplicationLayer>());
        return App;
    }

    bool DestroyApplication(СApplication *const App)
    {
        if (!App)
        {
            return false;
        }

        delete App;
        return true;
    }

} // namespace Corvus