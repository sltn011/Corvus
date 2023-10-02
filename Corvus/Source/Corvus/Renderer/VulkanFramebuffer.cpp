#include "CorvusPCH.h"

#include "Corvus/Renderer/Renderer.h"

namespace Corvus
{

    void CRenderer::CreateFramebuffers()
    {
        m_SwapchainFramebuffers.resize(m_SwapchainImageViews.size());

        for (size_t i = 0; i < m_SwapchainFramebuffers.size(); ++i)
        {
            std::array<VkImageView, 2> Attachments = {m_SwapchainImageViews[i], m_DepthImageView};

            VkFramebufferCreateInfo FramebufferInfo{};
            FramebufferInfo.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            FramebufferInfo.renderPass      = m_RenderPass;
            FramebufferInfo.attachmentCount = static_cast<UInt32>(Attachments.size());
            FramebufferInfo.pAttachments    = Attachments.data();
            FramebufferInfo.width           = m_SwapchainExtent.width;
            FramebufferInfo.height          = m_SwapchainExtent.height;
            FramebufferInfo.layers          = 1;

            if (vkCreateFramebuffer(m_Device, &FramebufferInfo, nullptr, &m_SwapchainFramebuffers[i]) != VK_SUCCESS)
            {
                CORVUS_CORE_CRITICAL("Failed to create Vulkan Framebuffer!");
            }
        }
        CORVUS_CORE_TRACE("Created Vulkan Framebuffers successfully");
    }

    void CRenderer::DestroyFramebuffers()
    {
        if (!m_SwapchainFramebuffers.empty())
        {
            for (size_t i = 0; i < m_SwapchainFramebuffers.size(); ++i)
            {
                vkDestroyFramebuffer(m_Device, m_SwapchainFramebuffers[i], nullptr);
            }
            CORVUS_CORE_TRACE("Vulkan Framebuffers destroyed");
        }
    }

} // namespace Corvus
