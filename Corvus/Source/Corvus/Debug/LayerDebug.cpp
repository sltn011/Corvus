#include "CorvusPCH.h"
#include "Corvus/Debug/LayerDebug.h"

namespace Corvus
{
    LayerDebug::LayerDebug(Name LayerName, bool bEnabled)
        : Super { LayerName, bEnabled }
    {
    }

    LayerDebug::~LayerDebug()
    {
    }

    void LayerDebug::OnUpdate()
    {
        if (!m_bEnabled)
        {
            return;
        }

        Super::OnUpdate();
    }

    void LayerDebug::OnEvent(EventBase &Event)
    {
        if (!m_bEnabled)
        {
            return;
        }

        Super::OnEvent(Event);
        CORVUS_CORE_TRACE("LayerDebug \"{0}\": {1}", m_LayerName, Event.ToString());
    }

}