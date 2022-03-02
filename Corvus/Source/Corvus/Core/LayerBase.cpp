#include "CorvusPCH.h"
#include "LayerBase.h"

namespace Corvus
{

    LayerBase::LayerBase(Name LayerName, bool bEnabled)
        : m_LayerName{LayerName}, m_bEnabled{bEnabled}
    {
        CORVUS_CORE_TRACE(STR("Layer {0} created and set to {1}"), m_LayerName, m_bEnabled ? STR("enabled") : STR("disabled"));
    }

    LayerBase::~LayerBase()
    {
    }

    void LayerBase::SetEnabled(bool bEnabled)
    {
        m_bEnabled = bEnabled;
        CORVUS_CORE_INFO(STR("Layer {0} {1}"), m_LayerName, m_bEnabled ? STR("enabled") : STR("disabled"));
    }

    bool LayerBase::IsEnabled() const
    {
        return m_bEnabled;
    }

    void LayerBase::OnPushed()
    {
        CORVUS_CORE_INFO(STR("Layer {0} pushed on layers stack!"), m_LayerName);
    }

    void LayerBase::OnPoped()
    {
        CORVUS_CORE_INFO(STR("Layer {0} poped of layers stack!"), m_LayerName);
    }

    void LayerBase::OnUpdate(float DeltaTime)
    {
    }

    void LayerBase::OnEvent(EventBase &Event)
    {
        CORVUS_CORE_TRACE(STR("Event {0} received by layer {1}"), Event.GetEventTypeString(), m_LayerName);
    }

}