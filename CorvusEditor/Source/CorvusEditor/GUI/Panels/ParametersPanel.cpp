#include "CorvusEditorPCH.h"

#include "CorvusEditor/GUI/Panels/ParametersPanel.h"

#include "Corvus/Components/TransformComponent.h"
#include "Corvus/Scene/Entity.h"

namespace Corvus
{

    void CParametersPanel::Render(FTimeDelta ElapsedTime, EPanelFlags PanelFlags)
    {
        ImGui::Begin("Parameters", nullptr, EnumRawValue(PanelFlags));

        if (m_SelectedEntity)
        {
            FTransform EntityTransform       = m_SelectedEntity->TransformComponent->GetTransform();
            FVector3   EntityPosition        = EntityTransform.GetPosition();
            FVector3   EntityScale           = EntityTransform.GetScale();
            FRotation  EntityRotation        = EntityTransform.GetRotation();
            FVector3   EntityRotationDegrees = EntityRotation.GetDegrees();

            bool bTransformChanged = false;

            if (Widgets::DrawVec3InputWidget("Position", EntityPosition, 0.1f))
            {
                EntityTransform.SetPosition(EntityPosition);
                bTransformChanged = true;
            }

            ImGui::NewLine();

            if (Widgets::DrawVec3InputWidget(
                    "Rotation", EntityRotationDegrees, 0.5f, true, {-180.0f, +180.0f}
                ))
            {
                EntityRotation.SetDegrees(EntityRotationDegrees);
                EntityTransform.SetRotation(EntityRotation);
                bTransformChanged = true;
            }
            ImGui::NewLine();

            ImGui::Text("Rotation Order");
            ERotationOrder               EntityRotationOrder   = EntityRotation.GetRotationOrder();
            static constexpr char const *RotationOrderLabels[] = {"XYZ", "XZY", "YXZ", "YZX", "ZXY", "ZYX"};
            char const *CurrentRotationLabel = RotationOrderLabels[EnumRawValue(EntityRotationOrder)];
            ImGui::SetNextItemWidth(60.0f);
            if (ImGui::BeginCombo("##Rotation Order", CurrentRotationLabel))
            {
                for (SizeT i = 0; i < 6; ++i)
                {
                    if (ImGui::Selectable(
                            RotationOrderLabels[i], RotationOrderLabels[i] == CurrentRotationLabel
                        ))
                    {
                        EntityRotationOrder = static_cast<ERotationOrder>(i);
                        EntityRotation.SetRotationOrder(EntityRotationOrder);
                        EntityTransform.SetRotation(EntityRotation);
                        bTransformChanged = true;
                    }
                }
                ImGui::EndCombo();
            }
            ImGui::NewLine();

            if (Widgets::DrawVec3InputWidget(
                    "Scale", EntityScale, 0.1f, true, {0.0f, Constants::Max<float>()}
                ))
            {
                EntityTransform.SetScale(EntityScale);
                bTransformChanged = true;
            }
            ImGui::NewLine();

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
