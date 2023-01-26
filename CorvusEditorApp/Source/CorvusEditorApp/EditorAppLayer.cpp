#include "CorvusEditorApp/EditorAppLayer.h"

#include "Corvus/Assets/Material/Material.h"
#include "Corvus/Assets/Model/ModelLoader.h"
#include "Corvus/Assets/Model/StaticModel.h"
#include "Corvus/Assets/Texture/Texture2D.h"
#include "Corvus/Camera/Camera.h"
#include "Corvus/Camera/PerspectiveCamera.h"
#include "Corvus/Components/StaticMeshComponent.h"
#include "Corvus/Components/TransformComponent.h"
#include "Corvus/Core/Application.h"
#include "Corvus/Event/ApplicationEvent.h"
#include "Corvus/Event/Event.h"
#include "Corvus/Event/KeyboardEvent.h"
#include "Corvus/Event/MouseEvent.h"
#include "Corvus/Input/Input.h"
#include "Corvus/Renderer/FrameBuffer.h"
#include "Corvus/Renderer/Renderer.h"
#include "Corvus/Renderer/Shader.h"
#include "Corvus/Scene/Entity.h"

// GUI
#include "CorvusEditor/GUI/Panels/AssetsPanel.h"
#include "CorvusEditor/GUI/Panels/ParametersPanel.h"
#include "CorvusEditor/GUI/Panels/ScenePanel.h"
#include "CorvusEditor/GUI/Panels/ViewportPanel.h"

namespace Corvus
{

    CEditorAppLayer::CEditorAppLayer() : CLayer{"Corvus Editor Layer", true}
    {
        CRenderer::EnableDepthTest();
        CRenderer::EnableBackfaceCulling();
        CRenderer::SetClearColor({0.6f, 0.8f, 1.0f, 1.0f});

        LoadAssets();
        CreateScene();
        WireUpAssets();

        CreateSceneFramebuffer();

        CreateEditorGUI();
    }

    void CEditorAppLayer::OnUpdate(FTimeDelta const ElapsedTime)
    {
        CRenderer::BeginScene();

        if (bRequestViewportResize)
        {
            ViewportSize = RequestedViewportSize;

            SceneFrameBuffer->Resize(ViewportSize.x, ViewportSize.y);
            CRenderer::ViewportResize(ViewportSize.x, ViewportSize.y);
            Scene.GetPlayerCamera()->SetViewportSize(
                static_cast<float>(ViewportSize.x), static_cast<float>(ViewportSize.y)
            );

            bRequestViewportResize = false;

            OnSceneFrameBufferChange.Broadcast(SceneFrameBuffer.get());
        }

        UpdateCamera(ElapsedTime);

        // Render to FrameBuffer
        CRenderer::SetRenderTarget(*SceneFrameBuffer);
        CRenderer::Clear();
        RenderScene(ElapsedTime);

        // Back to Window FrameBuffer
        CRenderer::SetDefaultRenderTarget();

        // CRenderer::Clear();
        // CRenderer::SubmitFrameBuffer(*SceneFrameBuffer);

        CRenderer::EndScene();
    }

    void CEditorAppLayer::OnEvent(CEvent &Event)
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
            FUIntVector2        NewSize = {
                static_cast<float>(WREvent.NewWidth), static_cast<float>(WREvent.NewHeight)};
            RequestSceneFramebufferResize(NewSize);
            // CPerspectiveCamera *Camera  = static_cast<CPerspectiveCamera *>(Scene.GetPlayerCamera());
            // Camera->SetViewportSize(
            //     static_cast<float>(ViewportSize.x), static_cast<float>(ViewportSize.x)
            //);
        }
        else if (Event.GetEventType() == CEvent::EEventType::MouseScroll)
        {
            CMouseScrollEvent &MSEvent = CastEvent<CMouseScrollEvent>(Event);
            if (bCameraMode)
            {
                CCamera    *Camera         = static_cast<CCamera *>(Scene.GetPlayerCamera());
                float const OldCameraSpeed = Camera->GetMoveSpeed();
                float const NewSpeed       = FMath::Max(OldCameraSpeed + MSEvent.OffsetY, 0.0f);
                Camera->SetMoveSpeed(NewSpeed);
                Event.SetHandled();
            }
        }
    }

    void CEditorAppLayer::OnGUIRender(FTimeDelta const ElapsedTime)
    {
        Dockspace.Render(ElapsedTime);
    }

    void CEditorAppLayer::CreateScene()
    {
        AddSceneCamera();
        PopulateScene();
    }

    void CEditorAppLayer::AddSceneCamera()
    {
        UInt32 const WindowWidth  = CApplication::GetInstance().GetWindow().GetWindowWidth();
        UInt32 const WindowHeight = CApplication::GetInstance().GetWindow().GetWindowHeight();

        TPoolable<CPerspectiveCamera> Camera = ConstructPoolable<CPerspectiveCamera>();
        Camera->SetViewportSize(static_cast<float>(WindowWidth), static_cast<float>(WindowHeight));
        Camera->SetFoVAngle(60.0f);
        Camera->SetClipPlanes(0.01f, 100.0f);
        Camera->SwitchPlayerControl(true, 1.0f);

        Scene.SetPlayerCamera(std::move(Camera));
    }

    void CEditorAppLayer::PopulateScene()
    {
        TPoolable<CEntity> Entity = ConstructPoolable<CEntity>();
        Entity->TransformComponent->SetPosition(FVector3{5.0f, -1.5f, 0.0f});
        Entity->TransformComponent->SetRotation(FRotation{{0.0f, -45.0f, 0.0f}});
        Entity->TransformComponent->SetScale(FVector3{1.0f});
        Entity->StaticMeshComponent->StaticModelRef.SetUUID(StaticModelsAssets.begin()->first);

        Scene.AddEntity(std::move(Entity));
    }

    void CEditorAppLayer::LoadAssets()
    {
        SStaticModelLoadedData LoadedModelData =
            CModelLoader::LoadStaticModelFromFile("./Assets/Models/Shack.glb");

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
            Material.CompileMaterialShader("./Assets/Shaders/TestShader.glsl");
            MaterialsAssets.emplace(Material.UUID, std::move(Material));
        }
    }

    void CEditorAppLayer::WireUpAssets()
    {
        // Provide materials with their textures
        for (auto &[MaterialUUID, Material] : MaterialsAssets)
        {
            if (Material.AlbedoMap.IsTexture())
            {
                FUUID AlbedoUUID = Material.AlbedoMap.TextureRef.GetUUID();
                Material.AlbedoMap.TextureRef.SetRawPtr(&TexturesAssets.at(AlbedoUUID));
            }
        }

        // Provide StaticMeshPrimitives with their materials
        for (auto &[StaticModelUUID, StaticModel] : StaticModelsAssets)
        {
            for (CStaticMesh &Mesh : StaticModel)
            {
                for (CStaticMeshPrimitive &Primitive : Mesh)
                {
                    FUUID MaterialUUID = Primitive.MaterialRef.GetUUID();
                    Primitive.MaterialRef.SetRawPtr(&MaterialsAssets.at(MaterialUUID));
                }
            }
        }

        // Provide Entities with their StaticModels
        for (TPoolable<CEntity> const &Entity : Scene.GetEntities())
        {
            FUUID StaticModelUUID = Entity->StaticMeshComponent->StaticModelRef.GetUUID();
            Entity->StaticMeshComponent->StaticModelRef.SetRawPtr(&StaticModelsAssets.at(StaticModelUUID));
        }
    }

    void CEditorAppLayer::UpdateCamera(FTimeDelta const ElapsedTime)
    {
        CCamera *Camera = Scene.GetPlayerCamera();
        if (!Camera)
        {
            CORVUS_NO_ENTRY_FMT("No Player Camera added to Scene!");
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
            Camera->ProcessRotationInput(Delta.x, Delta.y, 10.0f, ElapsedTime);
        }
    }

    void CEditorAppLayer::RenderScene(FTimeDelta const ElapsedTime)
    {
        for (TPoolable<CEntity> const &Entity : Scene.GetEntities())
        {
            CRenderer::SubmitStaticModel(
                *Entity->StaticMeshComponent->StaticModelRef.GetRawPtr(),
                Entity->TransformComponent->GetTransformMatrix(),
                Scene.GetPlayerCamera()->GetProjectionViewMatrix()
            );
        }
    }

    void CEditorAppLayer::CreateSceneFramebuffer()
    {
        ViewportSize = CApplication::GetInstance().GetWindow().GetWindowSize();

        std::vector<TOwn<CTexture2DBuffer>> TestFrameBufferAttachment(1);

        STextureDataFormat ScreenQuadFormat{ViewportSize.x, ViewportSize.y, EPixelFormat::RGBA8};
        STextureParameters ScreenQuadParameters{};
        TestFrameBufferAttachment[0] = CTexture2DBuffer::CreateEmpty(ScreenQuadFormat, ScreenQuadParameters);

        SceneFrameBuffer =
            CFrameBuffer::Create(ViewportSize.x, ViewportSize.y, std::move(TestFrameBufferAttachment));
    }

    void CEditorAppLayer::CreateEditorGUI()
    {
        Dockspace.AddPanel(CPanel::Create<CAssetsPanel>());

        TOwn<CParametersPanel> ParametersPanel = CPanel::Create<CParametersPanel>();
        OnEntitySelected.BindObject(ParametersPanel.get(), &CParametersPanel::SetSelectedEntity);
        Dockspace.AddPanel(std::move(ParametersPanel));

        TOwn<CScenePanel> ScenePanel = CPanel::Create<CScenePanel>(&Scene);
        ScenePanel->OnScenePanelSelectedEntityChange.BindObject(
            this, &CEditorAppLayer::BroadcastEntitySelection
        );
        OnSceneChange.BindObject(ScenePanel.get(), &CScenePanel::SetScene);
        OnEntitySelected.BindObject(ScenePanel.get(), &CScenePanel::SetSelectedEntity);
        Dockspace.AddPanel(std::move(ScenePanel));

        TOwn<CViewportPanel> ViewportPanel = CPanel::Create<CViewportPanel>(SceneFrameBuffer.get());
        ViewportPanel->OnViewportPanelResize.BindObject(
            this, &CEditorAppLayer::RequestSceneFramebufferResize
        );
        OnSceneFrameBufferChange.BindObject(ViewportPanel.get(), &CViewportPanel::SetViewportFramebuffer);
        Dockspace.AddPanel(std::move(ViewportPanel));
    }

    void CEditorAppLayer::RequestSceneFramebufferResize(FUIntVector2 NewSize)
    {
        RequestedViewportSize  = NewSize;
        bRequestViewportResize = true;
    }

    void CEditorAppLayer::BroadcastEntitySelection(CEntity const *SelectedEntityPtr) const
    {
        if (SelectedEntityPtr == nullptr) // for resetting selections
        {
            OnEntitySelected.Broadcast(nullptr);
        }

        TArray<TPoolable<CEntity>> const &Entities = Scene.GetEntities();
        for (SizeT EntityIndex = 0; EntityIndex < Entities.GetSize(); ++EntityIndex)
        {
            CEntity *EntityPtr = Entities[EntityIndex].Get();
            if (EntityPtr == SelectedEntityPtr)
            {
                OnEntitySelected.Broadcast(EntityPtr);
                return;
            }
        }
    }

} // namespace Corvus
