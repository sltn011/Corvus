#include "CorvusEditorPCH.h"

#include "CorvusEditor/GUI/Overlays/GizmoOverlay.h"

#include "Corvus/Camera/Camera.h"
#include "Corvus/Scene/Entity.h"

namespace Corvus
{

    CGizmoOverlay::CGizmoOverlay(CCamera *CurrentCamera)
    {
        SetCurrentCamera(CurrentCamera);
    }

    void CGizmoOverlay::Render(FTimeDelta ElapsedTime)
    {
    }

    void CGizmoOverlay::SetCurrentCamera(CCamera const *CurrentCamera)
    {
        CORVUS_CORE_ASSERT(CurrentCamera != nullptr);
        m_CurrentCamera = CurrentCamera;
    }

    void CGizmoOverlay::SetSelectedEntity(CEntity *SelectedEntity)
    {
        m_SelectedEntity = SelectedEntity;
    }

} // namespace Corvus