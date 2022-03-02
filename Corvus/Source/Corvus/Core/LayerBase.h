#ifndef CORVUS_SOURCE_CORVUS_CORE_LAYERBASE_H
#define CORVUS_SOURCE_CORVUS_CORE_LAYERBASE_H

#include "Corvus/Core/CoreTypes.h"
#include "Corvus/Events/EventBase.h"

namespace Corvus
{

    class LayerBase
    {
    public:

        LayerBase(Name LayerName, bool bEnabled);
        virtual ~LayerBase();

        void SetEnabled(bool bEnabled);
        bool IsEnabled() const;

        virtual void OnPushed();
        virtual void OnPoped();

        virtual void OnUpdate(float DeltaTime);

        virtual void OnEvent(EventBase &Event);

    protected:

        Name m_LayerName;
        bool m_bEnabled;
    };

}

#endif // !CORVUS_SOURCE_CORVUS_CORE_LAYERBASE_H
