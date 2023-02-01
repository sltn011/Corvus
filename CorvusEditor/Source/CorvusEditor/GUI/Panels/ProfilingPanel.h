#ifndef CORVUSEDITOR_SOURCE_CORVUSEDITOR_SOURCE_GUI_PANELS_PROFILINGPANEL_H
#define CORVUSEDITOR_SOURCE_CORVUSEDITOR_SOURCE_GUI_PANELS_PROFILINGPANEL_H

#include "CorvusEditor/GUI/Panels/Panel.h"

namespace Corvus
{

    class CProfilingPanel : public CPanel
    {
    public:
        using Super = CPanel;

        virtual void Render(FTimeDelta ElapsedTime, EPanelFlags PanelFlags) override;
    };

} // namespace Corvus

#endif // !CORVUSEDITOR_SOURCE_CORVUSEDITOR_SOURCE_GUI_PANELS_PROFILINGPANEL_H