#ifndef CORVUSEDITOR_SOURCE_CORVUSEDITOR_SOURCE_GUI_PANELS_VIEWPORTPANEL_H
#define CORVUSEDITOR_SOURCE_CORVUSEDITOR_SOURCE_GUI_PANELS_VIEWPORTPANEL_H

#include "Corvus/Core/Delegate.h"
#include "Corvus/Math/Vector.h"
#include "CorvusEditor/GUI/Panels/Panel.h"

namespace Corvus
{

    class CFrameBuffer;

    CORVUS_DECLARE_MULTICAST_DELEGATE(COnViewportPanelResize, FUIntVector2);

    class CViewportPanel : public CPanel
    {
    public:
        using Super = CPanel;

        CViewportPanel(CFrameBuffer const *ViewportFrameBufferPtr);

        virtual void Render(FTimeDelta ElapsedTime, EPanelFlags PanelFlags) override;

        void SetViewportFramebuffer(CFrameBuffer const *ViewportFrameBufferPtr);

    public:
        COnViewportPanelResize OnViewportPanelResize;

    private:
        CFrameBuffer const *m_ViewportFrameBufferPtr;
    };

} // namespace Corvus

#endif // !CORVUSEDITOR_SOURCE_CORVUSEDITOR_SOURCE_GUI_PANELS_VIEWPORTPANEL_H