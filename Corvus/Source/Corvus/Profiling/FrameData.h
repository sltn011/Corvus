#ifndef CORVUS_SOURCE_CORVUS_PROFILING_FRAMEDATA_H
#define CORVUS_SOURCE_CORVUS_PROFILING_FRAMEDATA_H

#include "Corvus/Core/CoreTypes.h"
#include "Corvus/Time/TimeDelta.h"

namespace Corvus
{
    struct SRenderData
    {
        UInt64 VerticesDrawn  = 0;
        UInt64 TrianglesDrawn = 0;
        UInt64 DrawCalls      = 0;
    };

    class CFrameData
    {
    public:
        CFrameData() = default;

        SRenderData RenderData     = {};
        FTimeDelta  ProcessingTime = {};
    };

} // namespace Corvus

#endif //! CORVUS_SOURCE_CORVUS_PROFILING_FRAMEDATA_H