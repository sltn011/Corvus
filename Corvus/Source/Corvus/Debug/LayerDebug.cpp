#include "CorvusPCH.h"

#include "Corvus/Debug/LayerDebug.h"

#include "Corvus/Time/TimeDelta.h"

namespace Corvus
{
    СLayerDebug::СLayerDebug(CString LayerName, bool const bEnabled) : Super{std::move(LayerName), bEnabled}
    {
    }

    void СLayerDebug::OnUpdate(FTimeDelta const ElapsedTime)
    {
        if (!m_bEnabled)
        {
            return;
        }
    }

    void СLayerDebug::OnEvent(CEvent &Event)
    {
        if (!m_bEnabled)
        {
            return;
        }

        CORVUS_CORE_TRACE("СLayerDebug \"{0}\": {1}", m_LayerName, Event.ToString());
    }

} // namespace Corvus