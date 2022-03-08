#ifndef CORVUS_SOURCE_CORVUS_CORE_LAYERBASE_H
#define CORVUS_SOURCE_CORVUS_CORE_LAYERBASE_H

#include "Corvus/Core/CoreTypes.h"
#include "Corvus/Events/EventBase.h"

namespace Corvus
{

    class LayerBase
    {
    public:

        LayerBase(String LayerName, bool bEnabled);
        virtual ~LayerBase();

        void SetEnabled(bool bEnabled);
        bool IsEnabled() const;

        virtual void OnPushed();
        virtual void OnPoped();

        virtual void OnUpdate() = 0;
        virtual void OnEvent(EventBase &Event) = 0;

    protected:

        String m_LayerName;
        bool m_bEnabled;
    };

}

#endif // !CORVUS_SOURCE_CORVUS_CORE_LAYERBASE_H
