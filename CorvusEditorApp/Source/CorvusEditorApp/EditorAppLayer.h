#ifndef CORVUSEDITORAPP_SOURCE_EDITORAPPLAYER_H
#define CORVUSEDITORAPP_SOURCE_EDITORAPPLAYER_H

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

    private:
        GUI::CDockspace Dockspace;

        CScene Scene;

        std::unordered_map<FUUID, CTexture2D>   TexturesAssets;
        std::unordered_map<FUUID, CMaterial>    MaterialsAssets;
        std::unordered_map<FUUID, CStaticModel> StaticModelsAssets;

        TOwn<CFrameBuffer>        SceneFrameBuffer;
        COnSceneFrameBufferChange OnSceneFrameBufferChange;

        FUIntVector2 ViewportSize{};
        FUIntVector2 RequestedViewportSize{};
        bool         bRequestViewportResize = false;

        bool     bCameraMode = false;
        FVector2 CursorPos;
    };

} // namespace Corvus

#endif // !CORVUSEDITORAPP_SOURCE_EDITORAPPLAYER_H