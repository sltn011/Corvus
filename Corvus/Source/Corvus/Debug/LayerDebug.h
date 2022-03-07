#ifndef CORVUS_SOURCE_CORVUS_DEBUG_LAYERDEBUG_H
#define CORVUS_SOURCE_CORVUS_DEBUG_LAYERDEBUG_H

#include "Corvus/Core/LayerBase.h"

namespace Corvus
{

    class LayerDebug : public LayerBase
    {
    public:

        using Super = LayerBase;

        LayerDebug(Name LayerName, bool bEnabled);
        virtual ~LayerDebug();

        virtual void OnUpdate() override;
        virtual void OnEvent(EventBase &Event) override;

    };

}

#endif // !CORVUS_SOURCE_CORVUS_DEBUG_LAYERDEBUG_H
