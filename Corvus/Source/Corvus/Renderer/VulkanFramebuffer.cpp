#include "CorvusPCH.h"

#include "Corvus/Renderer/Renderer.h"

namespace Corvus
{

    void CRenderer::CreateFramebuffers()
    {
        m_SwapchainFramebuffers.resize(m_SwapchainImageViews.size());

        for (size_t i = 0; i < m_SwapchainFramebuffers.size(); ++i)
        {
            VkImageView Attachments[] = {m_SwapchainImageViews[i]};

            VkFramebufferCreateInfo FramebufferInfo{};
            FramebufferInfo.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            FramebufferInfo.renderPass      = m_RenderPass;
            FramebufferInfo.attachmentCount = 1;
            FramebufferInfo.pAttachments    = Attachments;
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
