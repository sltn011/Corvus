#include <Corvus.h>

#include "Corvus/Assets/Material/Material.h"
#include "Corvus/Assets/Model/ModelLoader.h"
#include "Corvus/Assets/Model/StaticModel.h"
#include "Corvus/Assets/Texture/Texture2D.h"
#include "Corvus/Components/StaticMeshComponent.h"
#include "Corvus/Components/TransformComponent.h"
#include "Corvus/Scene/Entity.h"
#include "Corvus/Scene/Scene.h"

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
            LoadAssets();
            CreateScene();
            WireUpAssets();
        }

        ~CApplicationLayer() {}

        virtual void OnUpdate(FTimeDelta const ElapsedTime)
        {
            CCamera *Camera = CApplication::GetInstance().Scene.GetPlayerCamera();
            if (!Camera)
            {
                CORVUS_NO_ENTRY_FMT("No Player Camera added to Playground Scene!");
            }

            if (bCameraMode)
            {
                if (CInput::IsKeyPressed(Key::W))
                {
                    Camera->ProcessMovementInput(CCamera::EMoveDirection::Forward, ElapsedTime);
                }
                if (CInput::IsKeyPressed(Key::A))
                {
                    Camera->ProcessMovementInput(CCamera::EMoveDirection::Left, ElapsedTime);
                }
                if (CInput::IsKeyPressed(Key::S))
                {
                    Camera->ProcessMovementInput(CCamera::EMoveDirection::Backward, ElapsedTime);
                }
                if (CInput::IsKeyPressed(Key::D))
                {
                    Camera->ProcessMovementInput(CCamera::EMoveDirection::Right, ElapsedTime);
                }
                if (CInput::IsKeyPressed(Key::Space))
                {
                    Camera->ProcessMovementInput(CCamera::EMoveDirection::Up, ElapsedTime);
                }
                if (CInput::IsKeyPressed(Key::LeftShift))
                {
                    Camera->ProcessMovementInput(CCamera::EMoveDirection::Down, ElapsedTime);
                }
            }

            FVector2 const NewPos = CInput::GetCursorPos();
            FVector2 const Delta  = NewPos - CursorPos;
            CursorPos             = NewPos;
            if (bCameraMode)
            {
                Camera->ProcessRotationInput(Delta.x, Delta.y, 100.0f, ElapsedTime);
            }

            for (TPoolable<CEntity> const &Entity : CApplication::GetInstance().Scene.GetEntities())
            {
                CRenderer::GetInstance().SubmitStaticModel(
                    *Entity->StaticMeshComponent->StaticModelRef.GetRawPtr(),
                    Entity->StaticMeshComponent->GetTransformMatrix()
                );
            }
        }

        virtual void OnEvent(CEvent &Event)
        {
            if (Event.GetEventType() == CEvent::EEventType::MouseButtonPress)
            {
                CMouseButtonPressEvent &MBPEvent = CastEvent<CMouseButtonPressEvent>(Event);
                if (MBPEvent.Button == Mouse::ButtonRight)
                {
                    bCameraMode = true;
                    CInput::SetCursorEnabled(!bCameraMode);

                    Event.SetHandled();
                }
            }
            else if (Event.GetEventType() == CEvent::EEventType::MouseButtonRelease)
            {
                CMouseButtonReleaseEvent &MBREvent = CastEvent<CMouseButtonReleaseEvent>(Event);
                if (MBREvent.Button == Mouse::ButtonRight)
                {
                    bCameraMode = false;
                    CInput::SetCursorEnabled(!bCameraMode);

                    Event.SetHandled();
                }
            }
            else if (Event.GetEventType() == CEvent::EEventType::WindowResize)
            {
                CWindowResizeEvent &WREvent = CastEvent<CWindowResizeEvent>(Event);
                CPerspectiveCamera *Camera =
                    static_cast<CPerspectiveCamera *>(CApplication::GetInstance().Scene.GetPlayerCamera());
                Camera->SetViewportSize(static_cast<float>(WREvent.NewWidth), static_cast<float>(WREvent.NewHeight));
            }
            else if (Event.GetEventType() == CEvent::EEventType::MouseScroll)
            {
                CMouseScrollEvent &MSEvent = CastEvent<CMouseScrollEvent>(Event);
                if (bCameraMode)
                {
                    CCamera    *Camera = static_cast<CCamera *>(CApplication::GetInstance().Scene.GetPlayerCamera());
                    float const OldCameraSpeed = Camera->GetMoveSpeed();
                    float const NewSpeed       = FMath::Max(OldCameraSpeed + MSEvent.OffsetY, 0.0f);
                    Camera->SetMoveSpeed(NewSpeed);
                    Event.SetHandled();
                }
            }
        }

        void CreateScene()
        {
            AddSceneCamera();
            PopulateScene();
        }

        void AddSceneCamera()
        {
            UInt32 const WindowWidth  = CApplication::GetInstance().GetWindow().GetWindowWidth();
            UInt32 const WindowHeight = CApplication::GetInstance().GetWindow().GetWindowHeight();

            TPoolable<CPerspectiveCamera> Camera = ConstructPoolable<CPerspectiveCamera>();
            Camera->SetViewportSize(static_cast<float>(WindowWidth), static_cast<float>(WindowHeight));
            Camera->SetFoVAngle(60.0f);
            Camera->SetClipPlanes(0.01f, 100.0f);
            Camera->SwitchPlayerControl(true, 1.0f);

            CApplication::GetInstance().Scene.SetPlayerCamera(std::move(Camera));
        }

        void PopulateScene()
        {
            TPoolable<CEntity> Entity = ConstructPoolable<CEntity>();
            Entity->TransformComponent->SetPosition(FVector3{5.0f, -1.5f, 0.0f});
            Entity->TransformComponent->SetRotation(FRotation{{0.0f, -45.0f, 0.0f}});
            Entity->TransformComponent->SetScale(FVector3{0.01f});
            Entity->StaticMeshComponent->StaticModelRef.SetUUID(StaticModelsAssets.begin()->first);

            CApplication::GetInstance().Scene.AddEntity(std::move(Entity));
        }

        void LoadAssets()
        {
            SStaticModelLoadedData LoadedModelData =
                CModelLoader::LoadStaticModelFromFile("./Assets/Models/sponza.glb");

            // StaticModel
            StaticModelsAssets.emplace(LoadedModelData.StaticModel.UUID, std::move(LoadedModelData.StaticModel));

            // Textures
            for (CTexture2D &Texture : LoadedModelData.Textures)
            {
                TexturesAssets.emplace(Texture.UUID, std::move(Texture));
            }

            // Materials
            for (CMaterial &Material : LoadedModelData.Materials)
            {
                MaterialsAssets.emplace(Material.UUID, std::move(Material));
            }
        }

        void WireUpAssets()
        {
            // Provide materials with their textures
            for (auto &[MaterialUUID, Material] : MaterialsAssets)
            {
                FUUID AlbedoUUID = Material.Albedo.UUID;
                Material.Albedo  = TexturesAssets.at(AlbedoUUID);
            }

            // Provide StaticMeshPrimitives with their materials
            for (auto &[StaticModelUUID, StaticModel] : StaticModelsAssets)
            {
                for (CStaticMesh &Mesh : StaticModel)
                {
                    for (CStaticMeshPrimitive &Primitive : Mesh)
                    {
                        FUUID MaterialUUID = Primitive.Material.UUID;
                        Primitive.Material = MaterialsAssets.at(MaterialUUID);
                        Renderer().CreateMaterialRenderData(Primitive.Material);
                    }
                }
            }

            // Provide Entities with their StaticModels
            for (TPoolable<CEntity> const &Entity : CApplication::GetInstance().Scene.GetEntities())
            {
                FUUID StaticModelUUID = Entity->StaticMeshComponent->StaticModelRef.GetUUID();
                Entity->StaticMeshComponent->StaticModelRef.SetRawPtr(&StaticModelsAssets.at(StaticModelUUID));
            }
        }

    private:
        std::unordered_map<FUUID, CTexture2D>   TexturesAssets;
        std::unordered_map<FUUID, CMaterial>    MaterialsAssets;
        std::unordered_map<FUUID, CStaticModel> StaticModelsAssets;

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