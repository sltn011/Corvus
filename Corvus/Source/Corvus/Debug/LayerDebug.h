#ifndef CORVUS_SOURCE_CORVUS_DEBUG_LAYERDEBUG_H
#define CORVUS_SOURCE_CORVUS_DEBUG_LAYERDEBUG_H

#include "Corvus/Core/Layer.h"

namespace Corvus
{

    class TimeDelta;

    class LayerDebug : public Layer
    {
    public:

        using Super = Layer;

        LayerDebug(String LayerName, bool bEnabled);
        virtual ~LayerDebug();

        virtual void OnUpdate(TimeDelta ElapsedTime) override;
        virtual void OnEvent(Event &Event) override;

    };

}

#endif // !CORVUS_SOURCE_CORVUS_DEBUG_LAYERDEBUG_H
