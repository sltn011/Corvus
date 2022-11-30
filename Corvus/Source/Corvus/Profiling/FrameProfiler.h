#ifndef CORVUS_SOURCE_CORVUS_PROFILING_FRAMEPROFILER_H
#define CORVUS_SOURCE_CORVUS_PROFILING_FRAMEPROFILER_H

#include "Corvus/Profiling/FrameData.h"

namespace Corvus
{

    class CFrameProfiler
    {
    public:
        static void StartFrame();
        static void StopFrame();

        static void RecordRenderCallData(SRenderData const &RenderCallData);
        static void RecordFrameProcessingTime(FTimeDelta const &ElapsedTime);

        static CFrameData CurrentFrameData;
        static CFrameData PreviousFrameData;

#ifdef CORVUS_ENABLE_PROFILING
        static constexpr bool IsEnabled = true;
#else
        static constexpr bool IsEnabled = false;
#endif
    };

} // namespace Corvus

#endif //! CORVUS_SOURCE_CORVUS_PROFILING_FRAMEPROFILER_H