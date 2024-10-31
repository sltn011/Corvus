#include "CorvusPCH.h"

#include "Corvus/Renderer/Renderer.h"

namespace Corvus
{

    VkFramebuffer CRenderer::CreateFramebuffer(
        VkRenderPass RenderPass, VkExtent2D Extent, UInt32 Layers, std::vector<VkImageView> const &AttachmentsViews
    )
    {
        VkFramebuffer Framebuffer = VK_NULL_HANDLE;

        VkFramebufferCreateInfo FramebufferInfo =
            VkInit::FramebufferCreateInfo(RenderPass, Extent, Layers, AttachmentsViews.data(), AttachmentsViews.size());

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
            std::vector<VkImageView> Attachments = {SwapchainImageViews[i]};

            SwapchainFramebuffers[i] =
                CreateFramebuffer(RenderPass_Combine.RenderPass, SwapchainExtent, 1, Attachments);
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

    VkFramebuffer CRenderer::CurrentSwapchainFramebuffer()
    {
        CORVUS_CORE_ASSERT(SwapchainImageIndex < SwapchainFramebuffers.size());
        return SwapchainFramebuffers[SwapchainImageIndex];
    }

} // namespace Corvus
