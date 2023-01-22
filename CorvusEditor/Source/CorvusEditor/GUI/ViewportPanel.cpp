#include "CorvusEditorPCH.h"

#include "CorvusEditor/GUI/ViewportPanel.h"

namespace Corvus::GUI
{

    void CViewportPanel::Render(FTimeDelta ElapsedTime, EPanelFlags PanelFlags)
    {
        ImGui::Begin("Viewport", nullptr, RawValue(PanelFlags));
        /*ImVec2 RegionSize = ImGui::GetContentRegionAvail();
        ImGui::Image(
            SceneFrameBuffer->GetAttachment(0).GetTextureID(),
            {static_cast<float>(ViewportSize.x), static_cast<float>(ViewportSize.y)},
            {0.0f, 1.0f},
            {1.0f, 0.0f}
        );
        FUIntVector2 NewViewportSize{static_cast<UInt32>(RegionSize.x), static_cast<UInt32>(RegionSize.y)};
        if (ViewportSize != NewViewportSize)
        {
            ViewportSize     = NewViewportSize;
            bViewportUpdated = true;
        }*/
        ImGui::End();
    }

} // namespace Corvus::GUI
