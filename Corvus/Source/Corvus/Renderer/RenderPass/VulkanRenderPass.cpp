#include "CorvusPCH.h"

#include "Corvus/Renderer/Renderer.h"

namespace Corvus
{

    void CRenderer::CreateRenderPass()
    {
        CORVUS_ASSERT_FMT(RenderPass == VK_NULL_HANDLE, "Vulkan Render Pass was already created!");

        VkAttachmentDescription ColorAttachment = VkInit::AttachmentDescription(
            SwapchainImageFormat,
            VK_IMAGE_LAYOUT_UNDEFINED,
            VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
            VK_ATTACHMENT_LOAD_OP_CLEAR,
            VK_ATTACHMENT_STORE_OP_STORE,
            VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            VK_ATTACHMENT_STORE_OP_DONT_CARE
        );

        std::vector<VkAttachmentReference> ColorAttachmentsRef = {
            VkInit::AttachmentReference(0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)};

        VkAttachmentDescription DepthAttachment = VkInit::AttachmentDescription(
            FindDepthFormat(),
            VK_IMAGE_LAYOUT_UNDEFINED,
            VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
            VK_ATTACHMENT_LOAD_OP_CLEAR,
            VK_ATTACHMENT_STORE_OP_DONT_CARE, // won't be used after rendering
            VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            VK_ATTACHMENT_STORE_OP_DONT_CARE
        );

        VkAttachmentReference DepthAttachmentRef =
            VkInit::AttachmentReference(1, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

        std::vector<VkSubpassDescription> SubpassesDescription = {VkInit::SubpassDescription(
            VK_PIPELINE_BIND_POINT_GRAPHICS, ColorAttachmentsRef.data(), ColorAttachmentsRef.size(), &DepthAttachmentRef
        )};

        std::vector<VkSubpassDependency> DependenciesDescription = {VkInit::SubpassDependency(
            VK_SUBPASS_EXTERNAL,
            0,
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
            0,
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
            VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT
        )};

        std::vector<VkAttachmentDescription> AttachmentDescriptions = {ColorAttachment, DepthAttachment};

        VkRenderPassCreateInfo RenderPassInfo = VkInit::RenderPassCreateInfo(
            AttachmentDescriptions.data(),
            AttachmentDescriptions.size(),
            SubpassesDescription.data(),
            SubpassesDescription.size(),
            DependenciesDescription.data(),
            DependenciesDescription.size()
        );

        if (vkCreateRenderPass(Device, &RenderPassInfo, nullptr, &RenderPass) != VK_SUCCESS)
        {
            CORVUS_CORE_CRITICAL("Failed to create Vulkan Render Pass!");
        }
        CORVUS_CORE_TRACE("Created Vulkan Render Pass successfully");
    }

    void CRenderer::DestroyRenderPass()
    {
        if (RenderPass)
        {
            vkDestroyRenderPass(Device, RenderPass, nullptr);
            RenderPass = VK_NULL_HANDLE;
            CORVUS_CORE_TRACE("Vulkan Render Pass destroyed");
        }
    }

} // namespace Corvus
