#include "CorvusPCH.h"

#include "Corvus/Core/Layer.h"

#include "Corvus/Event/Event.h"
#include "Corvus/Time/TimeDelta.h"

namespace Corvus
{

    CLayer::CLayer(CString LayerName, bool const bEnabled)
        : m_LayerName{std::move(LayerName)}, m_bEnabled{bEnabled}
    {
        CORVUS_CORE_TRACE(
            "Layer \"{0}\" created and set to {1}", m_LayerName, m_bEnabled ? "enabled" : "disabled"
        );
    }

    void CLayer::SetEnabled(bool const bEnabled)
    {
        m_bEnabled = bEnabled;
        CORVUS_CORE_TRACE("Layer \"{0}\" {1}", m_LayerName, m_bEnabled ? "enabled" : "disabled");
    }

    bool CLayer::IsEnabled() const
    {
        return m_bEnabled;
    }

    void CLayer::OnPushed()
    {
        CORVUS_CORE_TRACE("Layer \"{0}\" pushed on layers stack!", m_LayerName);
    }

    void CLayer::OnPoped()
    {
        CORVUS_CORE_TRACE("Layer \"{0}\" poped of layers stack!", m_LayerName);
    }

    void CLayer::OnUpdate(FTimeDelta const ElapsedTime)
    {
    }

    void CLayer::OnEvent(CEvent &Event)
    {
    }

    void CLayer::Render()
    {
    }

} // namespace Corvus