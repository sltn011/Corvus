#ifndef CORVUSEDITOR_SOURCE_CORVUSEDITOR_SOURCE_GUI_VIEWPORTPANEL_H
#define CORVUSEDITOR_SOURCE_CORVUSEDITOR_SOURCE_GUI_VIEWPORTPANEL_H

#include "CorvusEditor/GUI/Panel.h"

namespace Corvus::GUI
{

    class CViewportPanel : public CPanel
    {
    public:
        using Super = CPanel;

        virtual void Render(FTimeDelta ElapsedTime, EPanelFlags PanelFlags) override;
    };

} // namespace Corvus::GUI

#endif // !CORVUSEDITOR_SOURCE_CORVUSEDITOR_SOURCE_GUI_VIEWPORTPANEL_H