#include "CorvusPCH.h"
#include "Corvus/Debug/LayerDebug.h"

#include "Corvus/Time/TimeDelta.h"

namespace Corvus
{
    LayerDebug::LayerDebug(String LayerName, bool bEnabled)
        : Super { LayerName, bEnabled }
    {
    }

    void LayerDebug::OnUpdate(TimeDelta ElapsedTime)
    {
        if (!m_bEnabled)
        {
            return;
        }
    }

    void LayerDebug::OnEvent(Event &Event)
    {
        if (!m_bEnabled)
        {
            return;
        }

        CORVUS_CORE_TRACE("LayerDebug \"{0}\": {1}", m_LayerName, Event.ToString());
    }

}