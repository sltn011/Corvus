#include "CorvusPCH.h"
#include "Corvus/Core/LayerBase.h"

namespace Corvus
{

    LayerBase::LayerBase(String LayerName, bool bEnabled)
        : m_LayerName{LayerName}, m_bEnabled{bEnabled}
    {
        CORVUS_CORE_TRACE("Layer \"{0}\" created and set to {1}", m_LayerName, m_bEnabled ? "enabled" : "disabled");
    }

    LayerBase::~LayerBase()
    {
    }

    void LayerBase::SetEnabled(bool bEnabled)
    {
        m_bEnabled = bEnabled;
        CORVUS_CORE_TRACE("Layer \"{0}\" {1}", m_LayerName, m_bEnabled ? "enabled" : "disabled");
    }

    bool LayerBase::IsEnabled() const
    {
        return m_bEnabled;
    }

    void LayerBase::OnPushed()
    {
        CORVUS_CORE_TRACE("Layer \"{0}\" pushed on layers stack!", m_LayerName);
    }

    void LayerBase::OnPoped()
    {
        CORVUS_CORE_TRACE("Layer \"{0}\" poped of layers stack!", m_LayerName);
    }

}