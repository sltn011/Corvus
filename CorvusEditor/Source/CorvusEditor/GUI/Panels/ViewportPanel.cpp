#include "CorvusEditorPCH.h"

#include "CorvusEditor/GUI/Panels/ViewportPanel.h"

#include "Corvus/Renderer/FrameBuffer.h"
#include "Corvus/Renderer/Texture2DBuffer.h"

namespace Corvus
{

    CViewportPanel::CViewportPanel(CFrameBuffer const *ViewportFrameBufferPtr)
    {
        SetViewportFramebuffer(ViewportFrameBufferPtr);
    }

    void CViewportPanel::Render(FTimeDelta ElapsedTime, EPanelFlags PanelFlags)
    {
        ImGui::Begin("Viewport", nullptr, RawValue(PanelFlags));

        ImVec2       RegionSize = ImGui::GetContentRegionAvail();
        FUIntVector2 ViewportSize{static_cast<UInt32>(RegionSize.x), static_cast<UInt32>(RegionSize.y)};

        FUIntVector2 FramebufferSize = m_ViewportFrameBufferPtr->GetSize();

        ImGui::Image(
            m_ViewportFrameBufferPtr->GetAttachment(0).GetTextureID(),
            {static_cast<float>(FramebufferSize.x), static_cast<float>(FramebufferSize.y)},
            {0.0f, 1.0f},
            {1.0f, 0.0f}
        );

        if (FramebufferSize != ViewportSize)
        {
            OnViewportPanelResize.Broadcast(ViewportSize);
        }

        ImGui::End();
    }

    void CViewportPanel::SetViewportFramebuffer(CFrameBuffer const *ViewportFrameBufferPtr)
    {
        CORVUS_CORE_ASSERT(ViewportFrameBufferPtr != nullptr);
        m_ViewportFrameBufferPtr = ViewportFrameBufferPtr;
    }

} // namespace Corvus
