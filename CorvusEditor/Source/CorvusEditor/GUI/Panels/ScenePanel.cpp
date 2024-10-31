#include "CorvusEditorPCH.h"

#include "CorvusEditor/GUI/Panels/ScenePanel.h"

#include "Corvus/Scene/Entity.h"
#include "Corvus/Scene/Scene.h"

namespace Corvus
{

    CScenePanel::CScenePanel(CScene const *ScenePtr)
    {
        SetScene(ScenePtr);
    }

    void CScenePanel::Render(FTimeDelta ElapsedTime, EPanelFlags PanelFlags)
    {
        if (ImGui::Begin("Scene", nullptr, EnumRawValue(PanelFlags)))
        {
            std::vector<TOwn<CEntity>> const &Entities = m_Scene->GetEntities();

            SizeT EntityIndex = 0;
            std::for_each(
                Entities.begin(),
                Entities.end(),
                [&](TOwn<CEntity> const &Entity)
                {
                    CString EntityName = "Entity " + std::to_string(EntityIndex);
                    if (ImGui::Selectable(EntityName.c_str(), Entity.get() == m_SelectedEntity))
                    {
                        SetSelectedEntity(Entity.get());
                    }
                }
            );
        }
        ImGui::End();
    }

    void CScenePanel::SetScene(CScene const *ScenePtr)
    {
        CORVUS_CORE_ASSERT(ScenePtr != nullptr);
        m_Scene = ScenePtr;
        SetSelectedEntity(nullptr);
    }

    void CScenePanel::SetSelectedEntity(CEntity const *SelectedEntityPtr)
    {
        if (SelectedEntityPtr == m_SelectedEntity)
        {
            return;
        }

        m_SelectedEntity = SelectedEntityPtr;
        OnScenePanelSelectedEntityChange.Broadcast(m_SelectedEntity);
    }

} // namespace Corvus
