#ifndef CORVUSEDITORAPP_SOURCE_EDITORAPPLAYER_H
#define CORVUSEDITORAPP_SOURCE_EDITORAPPLAYER_H

#include "Corvus/Assets/AssetDrawer.h"
#include "Corvus/Core/Base.h"
#include "Corvus/Core/Delegate.h"
#include "Corvus/Core/Layer.h"
#include "Corvus/Core/UUID.h"
#include "Corvus/Math/Vector.h"
#include "Corvus/Scene/Scene.h"

// GUI
#include "CorvusEditor/GUI/Dockspace.h"

namespace Corvus
{

    class CFrameBuffer;
    class CTexture2D;

    class CStaticModel;
    class CMaterial;

    CORVUS_DECLARE_MULTICAST_DELEGATE(COnSceneFrameBufferChange, CFrameBuffer const *);
    CORVUS_DECLARE_MULTICAST_DELEGATE(COnSceneChange, CScene const *);
    CORVUS_DECLARE_MULTICAST_DELEGATE(COnEntitySelected, CEntity *);
    CORVUS_DECLARE_MULTICAST_DELEGATE(COnCurrentCameraChange, CCamera *);

    class CEditorAppLayer : public CLayer
    {
    public:
        CEditorAppLayer();

        virtual void OnUpdate(FTimeDelta const ElapsedTime) override;
        virtual void OnEvent(CEvent &Event) override;
        virtual void OnGUIRender(FTimeDelta const ElapsedTime) override;

    private:
        void LoadAssets();

        void CreateScene();
        void AddSceneCamera();
        void PopulateScene();

        void WireUpAssets();

        void CreateSceneFramebuffer();

        void CreateEditorGUI();

        void UpdateCamera(FTimeDelta const ElapsedTime);
        void RenderScene(FTimeDelta const ElapsedTime);

        void RequestSceneFramebufferResize(FUIntVector2 NewSize);

        void BroadcastEntitySelection(CEntity const *SelectedEntityPtr) const;

    public:
        COnSceneChange         OnSceneChange;
        COnCurrentCameraChange OnCurrentCameraChange;

        COnSceneFrameBufferChange OnSceneFrameBufferChange;

        COnEntitySelected OnEntitySelected;

    private:
        CDockspace m_Dockspace;

        CScene m_Scene;

        CAssetDrawer m_AssetDrawer;

        TOwn<CFrameBuffer> m_SceneFrameBuffer;
        FUIntVector2       m_ViewportSize{};
        FUIntVector2       m_RequestedViewportSize{};
        bool               m_bRequestViewportResize = false;

        bool     m_bCameraMode = false;
        FVector2 m_CursorPos;
    };

} // namespace Corvus

#endif // !CORVUSEDITORAPP_SOURCE_EDITORAPPLAYER_H