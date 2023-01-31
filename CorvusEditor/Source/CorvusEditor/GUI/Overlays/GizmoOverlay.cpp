#include "CorvusEditorPCH.h"

#include "CorvusEditor/GUI/Overlays/GizmoOverlay.h"

#include "Corvus/Camera/Camera.h"
#include "Corvus/Components/TransformComponent.h"
#include "Corvus/Math/Quaternion.h"
#include "Corvus/Scene/Entity.h"

#include <ImGuizmo.h>

namespace Corvus
{

    CGizmoOverlay::CGizmoOverlay(CCamera *CurrentCamera)
    {
        SetCurrentCamera(CurrentCamera);
    }

    void CGizmoOverlay::Render(FTimeDelta ElapsedTime)
    {
        if (!m_SelectedEntity)
        {
            return;
        }

        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetDrawlist();

        ImVec2 PanelSize = ImGui::GetWindowSize();
        ImVec2 PanelPos  = ImGui::GetWindowPos();
        ImGuizmo::SetRect(PanelPos.x, PanelPos.y, PanelSize.x, PanelSize.y);

        FMatrix4 CameraView = m_CurrentCamera->GetViewMatrix();
        FMatrix4 CameraProj = m_CurrentCamera->GetProjectionMatrix();

        FTransform EntityTransform       = m_SelectedEntity->TransformComponent->GetTransform();
        FRotation  EntityRotation        = EntityTransform.GetRotation();
        FMatrix4   EntityTransformMatrix = EntityTransform.GetTransformMatrix();

        ImGuizmo::Manipulate(
            FMatrix::ValuePtr(CameraView),
            FMatrix::ValuePtr(CameraProj),
            ImGuizmo::OPERATION::UNIVERSAL,
            ImGuizmo::MODE::WORLD,
            FMatrix::ValuePtr(EntityTransformMatrix)
        );

        if (ImGuizmo::IsUsing())
        {
            FVector3    NewPosition{};
            FQuaternion NewRotationQuat{};
            FVector3    NewScale{};
            if (!FTransform::DecomposeTransform(
                    EntityTransformMatrix, NewPosition, NewRotationQuat, NewScale
                ))
            {
                CORVUS_CORE_ERROR("Failed to decompose transform matrix!");
                return;
            }

            EntityTransform.SetPosition(NewPosition);
            EntityTransform.SetRotation(FRotation{NewRotationQuat, EntityRotation.GetRotationOrder()});
            EntityTransform.SetScale(NewScale);

            m_SelectedEntity->TransformComponent->SetTransform(EntityTransform);
        }
    }

    void CGizmoOverlay::SetCurrentCamera(CCamera *CurrentCamera)
    {
        CORVUS_CORE_ASSERT(CurrentCamera != nullptr);
        m_CurrentCamera = CurrentCamera;
    }

    void CGizmoOverlay::SetSelectedEntity(CEntity *SelectedEntity)
    {
        m_SelectedEntity = SelectedEntity;
    }

} // namespace Corvus