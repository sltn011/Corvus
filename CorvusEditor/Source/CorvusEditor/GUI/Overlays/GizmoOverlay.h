#ifndef CORVUSEDITOR_SOURCE_CORVUSEDITOR_SOURCE_GUI_OVERLAYS_GIZMOOVERLAY_H
#define CORVUSEDITOR_SOURCE_CORVUSEDITOR_SOURCE_GUI_OVERLAYS_GIZMOOVERLAY_H

#include "CorvusEditor/GUI/Overlays/Overlay.h"

namespace Corvus
{

    class CCamera;
    class CEntity;

    class CGizmoOverlay : public COverlay
    {
    public:
        using Super = COverlay;

        CGizmoOverlay(CCamera *CurrentCamera);

        virtual void Render(FTimeDelta ElapsedTime) override;

        void SetCurrentCamera(CCamera const *CurrentCamera);

        void SetSelectedEntity(CEntity *SelectedEntity);

    private:
        CCamera const *m_CurrentCamera  = nullptr;
        CEntity       *m_SelectedEntity = nullptr;
    };

} // namespace Corvus

#endif // !CORVUSEDITOR_SOURCE_CORVUSEDITOR_SOURCE_GUI_OVERLAYS_GIZMOOVERLAY_H