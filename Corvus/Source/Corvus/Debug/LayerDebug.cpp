#include "CorvusPCH.h"
#include "Corvus/Debug/LayerDebug.h"

namespace Corvus
{
    LayerDebug::LayerDebug(String LayerName, bool bEnabled)
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
    }

    void LayerDebug::OnEvent(EventBase &Event)
    {
        if (!m_bEnabled)
        {
            return;
        }

        CORVUS_CORE_TRACE("LayerDebug \"{0}\": {1}", m_LayerName, Event.ToString());
    }

}