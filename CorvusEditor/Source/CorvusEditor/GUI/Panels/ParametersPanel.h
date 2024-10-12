#ifndef CORVUSEDITOR_SOURCE_CORVUSEDITOR_SOURCE_GUI_PANELS_PARAMETERSPANEL_H
#define CORVUSEDITOR_SOURCE_CORVUSEDITOR_SOURCE_GUI_PANELS_PARAMETERSPANEL_H

#include "CorvusEditor/GUI/Panels/Panel.h"

namespace Corvus
{

    class CEntity;

    class CParametersPanel : public CPanel
    {
    public:
        using Super = CPanel;

        virtual void Render(FTimeDelta ElapsedTime, EPanelFlags PanelFlags) override;

        void SetSelectedEntity(CEntity *SelectedEntityPtr);

    private:
        CEntity *m_SelectedEntity;
    };

} // namespace Corvus

#endif // !CORVUSEDITOR_SOURCE_CORVUSEDITOR_SOURCE_GUI_PANELS_PARAMETERSPANEL_H