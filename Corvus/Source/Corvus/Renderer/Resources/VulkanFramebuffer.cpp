#include "CorvusPCH.h"

#include "Corvus/Renderer/Renderer.h"

namespace Corvus
{

    VkFramebuffer CRenderer::CreateFramebuffer(
        VkRenderPass RenderPass, VkExtent2D Extent, UInt32 Layers, VkImageView *pAttachments, UInt32 NumAttachments
    )
    {
        VkFramebuffer Framebuffer = VK_NULL_HANDLE;

        VkFramebufferCreateInfo FramebufferInfo{};
        FramebufferInfo.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        FramebufferInfo.renderPass      = RenderPass;
        FramebufferInfo.attachmentCount = NumAttachments;
        FramebufferInfo.pAttachments    = pAttachments;
        FramebufferInfo.width           = Extent.width;
        FramebufferInfo.height          = Extent.height;
        FramebufferInfo.layers          = Layers;

        if (vkCreateFramebuffer(Device, &FramebufferInfo, nullptr, &Framebuffer) != VK_SUCCESS)
        {
            CORVUS_CORE_CRITICAL("Failed to create Vulkan Framebuffer!");
        }

        return Framebuffer;
    }

    void CRenderer::DestroyFramebuffer(VkFramebuffer &Framebuffer)
    {
        if (Framebuffer != VK_NULL_HANDLE)
        {
            vkDestroyFramebuffer(Device, Framebuffer, nullptr);
            Framebuffer = VK_NULL_HANDLE;
        }
    }

    void CRenderer::CreateFramebuffers()
    {
        SwapchainFramebuffers.resize(SwapchainImageViews.size());

        for (size_t i = 0; i < SwapchainFramebuffers.size(); ++i)
        {
            std::array<VkImageView, 1> Attachments = {SwapchainImageViews[i]};

            SwapchainFramebuffers[i] = CreateFramebuffer(
                RenderPass_Combine.RenderPass,
                SwapchainExtent,
                1,
                Attachments.data(),
                static_cast<UInt32>(Attachments.size())
            );
        }
        CORVUS_CORE_TRACE("Created Vulkan Framebuffers successfully");
    }

    void CRenderer::DestroyFramebuffers()
    {
        if (!SwapchainFramebuffers.empty())
        {
            for (size_t i = 0; i < SwapchainFramebuffers.size(); ++i)
            {
                DestroyFramebuffer(SwapchainFramebuffers[i]);
            }
            CORVUS_CORE_TRACE("Vulkan Framebuffers destroyed");
        }
    }

} // namespace Corvus
