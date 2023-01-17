#include "CorvusPCH.h"

#include "Corvus/Profiling/FrameProfiler.h"

namespace Corvus
{
    CFrameData CFrameProfiler::CurrentFrameData  = {};
    CFrameData CFrameProfiler::PreviousFrameData = {};

    void CFrameProfiler::StartFrame()
    {
        PreviousFrameData = CurrentFrameData;
        CurrentFrameData  = {};
    }

    void CFrameProfiler::StopFrame()
    {
        /*CORVUS_CORE_TRACE(
            "Frame Render Data - Vertices:{}, Triangles:{}, DrawCalls:{}, Time:{}ms",
            CurrentFrameData.RenderData.VerticesDrawn,
            CurrentFrameData.RenderData.TrianglesDrawn,
            CurrentFrameData.RenderData.DrawCalls,
            CurrentFrameData.ProcessingTime.MilliSeconds()
        );*/
    }

    void CFrameProfiler::RecordRenderCallData(SRenderData const &RenderCallData)
    {
        CurrentFrameData.RenderData.VerticesDrawn += RenderCallData.VerticesDrawn;
        CurrentFrameData.RenderData.TrianglesDrawn += RenderCallData.TrianglesDrawn;
        CurrentFrameData.RenderData.DrawCalls += RenderCallData.DrawCalls;
    }

    void CFrameProfiler::RecordFrameProcessingTime(FTimeDelta const &ElapsedTime)
    {
        CurrentFrameData.ProcessingTime = ElapsedTime;
    }

} // namespace Corvus
