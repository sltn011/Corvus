#ifndef CORVUS_SOURCE_CORVUS_DEBUG_LAYERDEBUG_H
#define CORVUS_SOURCE_CORVUS_DEBUG_LAYERDEBUG_H

#include "Corvus/Core/Layer.h"

namespace Corvus
{

    class FTimeDelta;

    class СLayerDebug : public СLayer
    {
    public:
        using Super = СLayer;

        СLayerDebug(CString LayerName, bool bEnabled);

        virtual void OnUpdate(FTimeDelta ElapsedTime) override;
        virtual void OnEvent(CEvent &Event) override;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_DEBUG_LAYERDEBUG_H
