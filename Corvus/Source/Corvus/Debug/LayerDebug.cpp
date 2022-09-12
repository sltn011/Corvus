#include "CorvusPCH.h"

#include "Corvus/Debug/LayerDebug.h"

#include "Corvus/Time/TimeDelta.h"

namespace Corvus
{
    CLayerDebug::CLayerDebug(CString LayerName, bool const bEnabled) : Super{std::move(LayerName), bEnabled}
    {
    }

    void CLayerDebug::OnUpdate(FTimeDelta const ElapsedTime)
    {
        if (!m_bEnabled)
        {
            return;
        }
    }

    void CLayerDebug::OnEvent(CEvent &Event)
    {
        if (!m_bEnabled)
        {
            return;
        }

        CORVUS_CORE_TRACE("CLayerDebug \"{0}\": {1}", m_LayerName, Event.ToString());
    }

} // namespace Corvus