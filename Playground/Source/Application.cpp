#include <Corvus.h>

#include "Corvus/Assets/Image/Image.h"
#include "Corvus/Assets/Image/ImageLoader.h"
#include "Corvus/Assets/Material/Material.h"
#include "Corvus/Assets/Model/ModelLoader.h"
#include "Corvus/Assets/Model/StaticModel.h"
#include "Corvus/Components/StaticMeshComponent.h"
#include "Corvus/Components/TransformComponent.h"
#include "Corvus/Math/Color.h"
#include "Corvus/Renderer/Texture2D.h"

namespace Corvus
{

    class CPlayground : public CApplication
    {
    public:
        CPlayground() {}
        ~CPlayground() {}
    };

    class CApplicationLayer : public CLayer
    {
    public:
        CApplicationLayer() : CLayer{"ApplicationLayer", true}
        {
            CRenderer::EnableDepthTest();
            CRenderer::SetClearColor({0.6f, 0.8f, 1.0f, 1.0f});

            InitCamera();
            // CreateMeshData();
            // PopulateScene();

            CreateTexture();
            CreateMaterial();

            CreateStaticMesh();
        }

        virtual void OnUpdate(FTimeDelta const ElapsedTime)
        {
            CRenderer::BeginScene();
            CRenderer::Clear();

            if (bCameraMode)
            {
                if (CInput::IsKeyPressed(Key::W))
                {
                    CCamera.ProcessMovementInput(CCamera::EMoveDirection::Forward, ElapsedTime);
                }
                if (CInput::IsKeyPressed(Key::A))
                {
                    CCamera.ProcessMovementInput(CCamera::EMoveDirection::Left, ElapsedTime);
                }
                if (CInput::IsKeyPressed(Key::S))
                {
                    CCamera.ProcessMovementInput(CCamera::EMoveDirection::Backward, ElapsedTime);
                }
                if (CInput::IsKeyPressed(Key::D))
                {
                    CCamera.ProcessMovementInput(CCamera::EMoveDirection::Right, ElapsedTime);
                }
                if (CInput::IsKeyPressed(Key::Space))
                {
                    CCamera.ProcessMovementInput(CCamera::EMoveDirection::Up, ElapsedTime);
                }
                if (CInput::IsKeyPressed(Key::LeftShift))
                {
                    CCamera.ProcessMovementInput(CCamera::EMoveDirection::Down, ElapsedTime);
                }
            }

            FVector2 const NewPos = CInput::GetCursorPos();
            FVector2 const Delta  = NewPos - CursorPos;
            CursorPos             = NewPos;
            if (bCameraMode)
            {
                CCamera.ProcessRotationInput(Delta.x, Delta.y, 10.0f, ElapsedTime);
            }

            TOwn<CShader> &MaterialShader = TestMaterial.GetShader();
            MaterialShader->Bind();
            MaterialShader->SetMat4("u_Transform", TestModelTransform.GetTransformMatrix());
            MaterialShader->SetMat4("u_ProjView", CCamera.GetProjectionViewMatrix());
            TestMaterial.LoadInShader();

            for (SizeT MeshIndex = 0; MeshIndex < TestModel.GetNumMeshes(); ++MeshIndex)
            {
                CStaticMesh &Mesh = TestModel.GetMesh(MeshIndex);
                for (SizeT PrimitiveIndex = 0; PrimitiveIndex < Mesh.GetNumPrimitives(); ++PrimitiveIndex)
                {
                    CStaticMeshPrimitive &Primitive = Mesh.GetPrimitive(PrimitiveIndex);
                    CRenderer::Submit(Primitive.GetVertexArray(), MaterialShader);
                }
            }

            CRenderer::EndScene();
        }

        virtual void OnEvent(CEvent &Event)
        {
            if (Event.GetEventType() == CEvent::EEventType::KeyPress)
            {
                CKeyPressEvent &KPEvent = CastEvent<CKeyPressEvent>(Event);
                if (KPEvent.Key == Key::C)
                {
                    bCameraMode = !bCameraMode;
                    CInput::SetCursorEnabled(!bCameraMode);

                    Event.SetHandled();
                }
            }
            else if (Event.GetEventType() == CEvent::EEventType::WindowResize)
            {
                CWindowResizeEvent &WREvent = CastEvent<CWindowResizeEvent>(Event);
                CCamera.SetViewportSize(static_cast<float>(WREvent.NewWidth), static_cast<float>(WREvent.NewHeight));
            }
        }

        void InitCamera()
        {
            UInt32 const WindowWidth  = CApplication::GetInstance().GetWindow().GetWindowWidth();
            UInt32 const WindowHeight = CApplication::GetInstance().GetWindow().GetWindowHeight();
            CCamera.SetViewportSize(static_cast<float>(WindowWidth), static_cast<float>(WindowHeight));
            CCamera.SetFoVAngle(60.0f);
            CCamera.SetClipPlanes(0.01f, 100.0f);
            CCamera.SwitchPlayerControl(true, 1.0f);
        }

        void CreateMeshData()
        {
            // clang-format off
            float const VerticesPositions[] = {
                +0.0f, -0.2f, -0.2f,
                +0.0f, -0.2f, +0.2f,
                +0.0f, +0.2f, +0.2f,
                +0.0f, +0.2f, -0.2f
            };

            float const VerticesUVs[] = {
                0.0f, 0.0f,
                1.0f, 0.0f,
                1.0f, 1.0f,
                0.0f, 1.0f
            };
            // clang-format on

            UInt32 const Indices[] = {0, 1, 2, 0, 2, 3};

            CVertexBufferLayout const VBOPosLayout = {{EBufferDataType::Vec3}};
            TOwn<CVertexBuffer>       VBOPos       = CVertexBuffer::Create(VerticesPositions, 4, VBOPosLayout);

            CVertexBufferLayout const VBOUVsLayout = {{EBufferDataType::Vec2}};
            TOwn<CVertexBuffer>       VBOUVs       = CVertexBuffer::Create(VerticesUVs, 4, VBOUVsLayout);

            TOwn<CIndexBuffer> EBO = CIndexBuffer::Create(Indices, 6);

            VAO = CVertexArray::Create();
            VAO->AddVertexBuffer(std::move(VBOPos));
            VAO->AddVertexBuffer(std::move(VBOUVs));
            VAO->AddIndexBuffer(std::move(EBO));
        }

        void PopulateScene()
        {
            Entities.EmplaceBack(
                VAO, FTransform{{1.0f, 0.0f, 0.0f}, FVector::OneVec, {ERotationOrder::YXZ, {30.0f, 0.0f, 0.0f}}}
            );

            Entities.EmplaceBack(
                VAO, FTransform{{0.0f, 0.0f, 0.5f}, FVector::OneVec * 0.5f, {ERotationOrder::YXZ, {0.0f, 0.0f, 45.0f}}}
            );

            Entities[0].TransformComponent->AddChild(Entities[1].TransformComponent.Get());
        }

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

            // TestMaterial.AlbedoMap.SetOther(FVector4{FColor::Magenta, 1.0f});
            // TestMaterial.AlbedoMap.UseOther();

            TestMaterial.CompileMaterialShader("./Assets/Shaders/TestShader.glsl");
        }

        void CreateStaticMesh()
        {
            TestModel = CModelLoader::LoadStaticModelFromFile("./Assets/Models/Shack.glb");

            TestModelTransform =
                FTransform{{5.0f, -1.5f, 0.0f}, FVector::OneVec, {ERotationOrder::YXZ, {0.0f, -45.0f, 0.0f}}};
        }

    protected:
        TArray<CEntity>    Entities;
        CPerspectiveCamera CCamera;

        TOwn<CVertexArray> VAO;
        TOwn<CTexture2D>   TestTexture;
        CMaterial          TestMaterial;

        CStaticModel TestModel;
        FTransform   TestModelTransform;

        bool     bCameraMode = false;
        FVector2 CursorPos;
    };

    CApplication *CreateApplication()
    {
        CPlayground *App = new CPlayground;
        App->PushLayer(CLayer::Create<CApplicationLayer>());
        return App;
    }

    bool DestroyApplication(CApplication *const App)
    {
        if (!App)
        {
            return false;
        }

        delete App;
        return true;
    }

} // namespace Corvus