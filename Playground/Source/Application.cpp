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

            for (SizeT MeshIndex = 0; MeshIndex < TestModelData.StaticModel.GetNumMeshes(); ++MeshIndex)
            {
                CStaticMesh &Mesh = TestModelData.StaticModel.GetMesh(MeshIndex);
                for (SizeT PrimitiveIndex = 0; PrimitiveIndex < Mesh.GetNumPrimitives(); ++PrimitiveIndex)
                {
                    CStaticMeshPrimitive &Primitive = Mesh.GetPrimitive(PrimitiveIndex);

                    CShader *PrimitiveShader = Primitive.GetMaterial().GetShader();
                    CORVUS_ASSERT(PrimitiveShader != nullptr);

                    PrimitiveShader->Bind();
                    PrimitiveShader->SetMat4("u_Transform", TestModelTransform.GetTransformMatrix());
                    PrimitiveShader->SetMat4("u_ProjView", CCamera.GetProjectionViewMatrix());
                    Primitive.GetMaterial().LoadInShader();

                    CRenderer::Submit(
                        Primitive.GetVertexArray().get(), PrimitiveShader
                    ); // Reminder of a changed signature
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

        void CreateStaticMesh()
        {
            TestModelData = CModelLoader::LoadStaticModelFromFile("./Assets/Models/Shack.glb");
            // TestModelData = CModelLoader::LoadStaticModelFromFile("./Assets/Models/3Cubes.glb");

            // Create separate shader for each primitive
            // Very ugly and bad way of doing it will be changed ASAP
            // Only for model loading tests

            SizeT NumPrimitives = 0;
            for (SizeT i = 0; i < TestModelData.StaticModel.GetNumMeshes(); ++i)
            {
                NumPrimitives += TestModelData.StaticModel.GetMesh(i).GetNumPrimitives();
            }
            TestShaders.resize(NumPrimitives);

            SizeT ShaderCnt = 0;
            for (SizeT i = 0; i < TestModelData.StaticModel.GetNumMeshes(); ++i)
            {
                CStaticMesh &StaticMesh = TestModelData.StaticModel.GetMesh(i);
                for (SizeT j = 0; j < StaticMesh.GetNumPrimitives(); ++j)
                {
                    CStaticMeshPrimitive &StaticMeshPrimitive = StaticMesh.GetPrimitive(j);

                    auto Parameters        = StaticMeshPrimitive.GetMaterial().GetShaderCompileParameters();
                    TestShaders[ShaderCnt] = CShader::CreateFromFile("./Assets/Shaders/TestShader.glsl", Parameters);
                    StaticMeshPrimitive.GetMaterial().SetShader(TestShaders[ShaderCnt].get());
                    ShaderCnt++;
                }
            }

            TestModelTransform =
                FTransform{{5.0f, -1.5f, 0.0f}, FVector::OneVec, {ERotationOrder::YXZ, {0.0f, -45.0f, 0.0f}}};
        }

    protected:
        TArray<CEntity>    Entities;
        CPerspectiveCamera CCamera;

        SStaticModelLoadedData     TestModelData;
        FTransform                 TestModelTransform;
        std::vector<TOwn<CShader>> TestShaders;

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