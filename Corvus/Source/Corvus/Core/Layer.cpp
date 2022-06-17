#include "CorvusPCH.h"
#include "Corvus/Core/Layer.h"

#include "Corvus/Events/Event.h"
#include "Corvus/Time/TimeDelta.h"

namespace Corvus
{

    Layer::Layer(String LayerName, bool bEnabled)
        : m_LayerName{LayerName}, m_bEnabled{bEnabled}
    {
        CORVUS_CORE_TRACE("Layer \"{0}\" created and set to {1}", m_LayerName, m_bEnabled ? "enabled" : "disabled");
    }

    void Layer::SetEnabled(bool bEnabled)
    {
        m_bEnabled = bEnabled;
        CORVUS_CORE_TRACE("Layer \"{0}\" {1}", m_LayerName, m_bEnabled ? "enabled" : "disabled");
    }

    bool Layer::IsEnabled() const
    {
        return m_bEnabled;
    }

    void Layer::OnPushed()
    {
        CORVUS_CORE_TRACE("Layer \"{0}\" pushed on layers stack!", m_LayerName);
    }

    void Layer::OnPoped()
    {
        CORVUS_CORE_TRACE("Layer \"{0}\" poped of layers stack!", m_LayerName);
    }

    void Layer::OnUpdate(TimeDelta ElapsedTime)
    {
    }

    void Layer::OnEvent(Event &Event)
    {
    }

    void Layer::Render()
    {
    }

}