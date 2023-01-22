#ifndef CORVUSEDITOR_SOURCE_CORVUSEDITOR_SOURCE_GUI_DOCKSPACE_H
#define CORVUSEDITOR_SOURCE_CORVUSEDITOR_SOURCE_GUI_DOCKSPACE_H

#include "Corvus/Core/Base.h"
#include "Corvus/Time/TimeDelta.h"

#include <vector>

namespace Corvus::GUI
{

    class CPanel;

    class CDockspace
    {
    public:
        SizeT AddPanel(TOwn<CPanel> &&Panel);

        SizeT GetNumPanels() const;

        CPanel &GetPanel(SizeT PanelIndex);

        void Render(FTimeDelta const &ElapsedTime);

    private:
        std::vector<TOwn<CPanel>> m_Panels;
    };

} // namespace Corvus::GUI

#endif // !CORVUSEDITOR_SOURCE_CORVUSEDITOR_SOURCE_GUI_DOCKSPACE_H