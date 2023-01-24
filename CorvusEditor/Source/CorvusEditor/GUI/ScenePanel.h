#ifndef CORVUSEDITOR_SOURCE_CORVUSEDITOR_SOURCE_GUI_SCENEPANEL_H
#define CORVUSEDITOR_SOURCE_CORVUSEDITOR_SOURCE_GUI_SCENEPANEL_H

#include "Corvus/Core/Delegate.h"
#include "CorvusEditor/GUI/Panel.h"

namespace Corvus
{

    class CEntity;
    class CScene;

    CORVUS_DECLARE_MULTICAST_DELEGATE(COnScenePanelSelectedEntityChange, CEntity const *);

    class CScenePanel : public CPanel
    {
    public:
        using Super = CPanel;

        CScenePanel(CScene const *ScenePtr);

        virtual void Render(FTimeDelta ElapsedTime, EPanelFlags PanelFlags) override;

        void SetScene(CScene const *ScenePtr);

        void SetSelectedEntity(CEntity const *SelectedEntityPtr);

    public:
        COnScenePanelSelectedEntityChange OnScenePanelSelectedEntityChange;

    private:
        CScene const  *m_Scene          = nullptr;
        CEntity const *m_SelectedEntity = nullptr;
    };

} // namespace Corvus

#endif // !CORVUSEDITOR_SOURCE_CORVUSEDITOR_SOURCE_GUI_SCENEPANEL_H