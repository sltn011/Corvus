#include "CorvusEditorPCH.h"

#include "CorvusEditor/GUI/Panels/ProfilingPanel.h"

#include "Corvus/Profiling/FrameProfiler.h"

namespace Corvus

{
    void CProfilingPanel::Render(FTimeDelta ElapsedTime, EPanelFlags PanelFlags)
    {
        if (ImGui::Begin("Profiling", nullptr, EnumRawValue(PanelFlags)))
        {
            CORVUS_EVAL_IF_CONSTEXPR(
                CFrameProfiler::IsEnabled,
                []()
                {
                    CFrameData const &PrevFrame = CFrameProfiler::PreviousFrameData;
                    ImGui::Text(
                        fmt::format("Frame Time: {}ms", PrevFrame.ProcessingTime.MilliSeconds()).c_str()
                    );
                    ImGui::NewLine();
                    ImGui::Text("Draw Info:");
                    ImGui::Text(fmt::format("Draw Calls: {}", PrevFrame.RenderData.DrawCalls).c_str());
                    ImGui::Text(fmt::format("Triangles: {}", PrevFrame.RenderData.TrianglesDrawn).c_str());
                    ImGui::Text(fmt::format("Vertices: {}", PrevFrame.RenderData.VerticesDrawn).c_str());
                }
            )
            else
            {
                ImGui::Text("Profiling disabled");
            }
        }
        ImGui::End();
    }

} // namespace Corvus
