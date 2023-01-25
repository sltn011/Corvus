#include "CorvusEditorPCH.h"

#include "CorvusEditor/GUI/ParametersPanel.h"

#include "Corvus/Components/TransformComponent.h"
#include "Corvus/Scene/Entity.h"

namespace Corvus
{

    void CParametersPanel::Render(FTimeDelta ElapsedTime, EPanelFlags PanelFlags)
    {
        ImGui::Begin("Parameters", nullptr, RawValue(PanelFlags));

        if (m_SelectedEntity)
        {
            FTransform EntityTransform     = m_SelectedEntity->TransformComponent->GetTransform();
            FVector3   EntityPosition      = EntityTransform.GetPosition();
            FVector3   EntityScale         = EntityTransform.GetScale();
            FRotation  EntityRotation      = EntityTransform.GetRotation();
            FVector3   EntityRotationEuler = EntityRotation.GetDegrees();

            bool bTransformChanged = false;

            if (ImGui::DragFloat3("Position", &EntityPosition.x, 0.25f))
            {
                EntityTransform.SetPosition(EntityPosition);
                bTransformChanged = true;
            }
            if (ImGui::DragFloat3("Scale", &EntityScale.x, 0.25f))
            {
                EntityTransform.SetScale(EntityScale);
                bTransformChanged = true;
            }
            if (ImGui::DragFloat3("Rotation", &EntityRotationEuler.x, 0.25f))
            {
                EntityRotation.SetDegrees(EntityRotationEuler);
                EntityTransform.SetRotation(EntityRotation);
                bTransformChanged = true;
            }

            if (bTransformChanged)
            {
                m_SelectedEntity->TransformComponent->SetTransform(EntityTransform);
            }
        }

        ImGui::End();
    }

    void CParametersPanel::SetSelectedEntity(CEntity *SelectedEntityPtr)
    {
        m_SelectedEntity = SelectedEntityPtr;
    }

} // namespace Corvus
