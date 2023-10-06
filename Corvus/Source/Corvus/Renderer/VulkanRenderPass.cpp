#include "CorvusPCH.h"

#include "Corvus/Renderer/Renderer.h"

namespace Corvus
{

    void CRenderer::CreateRenderPass()
    {
        CORVUS_ASSERT_FMT(m_RenderPass == VK_NULL_HANDLE, "Vulkan Render Pass was already created!");

        VkAttachmentDescription ColorAttachment{};
        ColorAttachment.format         = m_SwapchainImageFormat;
        ColorAttachment.samples        = VK_SAMPLE_COUNT_1_BIT;
        ColorAttachment.loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
        ColorAttachment.storeOp        = VK_ATTACHMENT_STORE_OP_STORE;
        ColorAttachment.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        ColorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        ColorAttachment.initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
        ColorAttachment.finalLayout    = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference ColorAttachmentRef{};
        ColorAttachmentRef.attachment = 0; // index of attachment in pAttachments array in RenderPassInfo
        ColorAttachmentRef.layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkAttachmentDescription DepthAttachment{};
        DepthAttachment.format         = FindDepthFormat();
        DepthAttachment.samples        = VK_SAMPLE_COUNT_1_BIT;
        DepthAttachment.loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
        DepthAttachment.storeOp        = VK_ATTACHMENT_STORE_OP_DONT_CARE; // won't be used after rendering
        DepthAttachment.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        DepthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        DepthAttachment.initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
        DepthAttachment.finalLayout    = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkAttachmentReference DepthAttachmentRef{};
        DepthAttachmentRef.attachment = 1;
        DepthAttachmentRef.layout     = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkSubpassDescription Subpass{};
        Subpass.pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS;
        Subpass.colorAttachmentCount    = 1;
        Subpass.pColorAttachments       = &ColorAttachmentRef;
        Subpass.pDepthStencilAttachment = &DepthAttachmentRef;

        VkSubpassDependency Dependency{};
        Dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        Dependency.dstSubpass = 0; // our only subpass
        Dependency.srcStageMask =
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        Dependency.srcAccessMask = 0;
        Dependency.dstStageMask =
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        Dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

        std::array<VkAttachmentDescription, 2> AttachmentDescriptions = {ColorAttachment, DepthAttachment};

        VkRenderPassCreateInfo RenderPassInfo{};
        RenderPassInfo.sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        RenderPassInfo.attachmentCount = static_cast<UInt32>(AttachmentDescriptions.size());
        RenderPassInfo.pAttachments    = AttachmentDescriptions.data();
        RenderPassInfo.subpassCount    = 1;
        RenderPassInfo.pSubpasses      = &Subpass;
        RenderPassInfo.dependencyCount = 1;
        RenderPassInfo.pDependencies   = &Dependency;

        if (vkCreateRenderPass(m_Device, &RenderPassInfo, nullptr, &m_RenderPass) != VK_SUCCESS)
        {
            CORVUS_CORE_CRITICAL("Failed to create Vulkan Render Pass!");
        }
        CORVUS_CORE_TRACE("Created Vulkan Render Pass successfully");
    }

    void CRenderer::DestroyRenderPass()
    {
        if (m_RenderPass)
        {
            vkDestroyRenderPass(m_Device, m_RenderPass, nullptr);
            m_RenderPass = VK_NULL_HANDLE;
            CORVUS_CORE_TRACE("Vulkan Render Pass destroyed");
        }
    }

    void CRenderer::CreateGUIRenderPass()
    {
        CORVUS_ASSERT_FMT(m_GUIRenderPass == VK_NULL_HANDLE, "Vulkan GUI Render Pass was already created!");

        VkAttachmentDescription ColorAttachment{};
        ColorAttachment.format         = m_SwapchainImageFormat;
        ColorAttachment.samples        = VK_SAMPLE_COUNT_1_BIT;
        ColorAttachment.loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
        ColorAttachment.storeOp        = VK_ATTACHMENT_STORE_OP_STORE;
        ColorAttachment.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        ColorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        ColorAttachment.initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
        ColorAttachment.finalLayout    = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference ColorAttachmentRef{};
        ColorAttachmentRef.attachment = 0; // index of attachment in pAttachments array in RenderPassInfo
        ColorAttachmentRef.layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription Subpass{};
        Subpass.pipelineBindPoint    = VK_PIPELINE_BIND_POINT_GRAPHICS;
        Subpass.colorAttachmentCount = 1;
        Subpass.pColorAttachments    = &ColorAttachmentRef;

        VkSubpassDependency Dependency{};
        Dependency.srcSubpass    = VK_SUBPASS_EXTERNAL;
        Dependency.dstSubpass    = 0; // our only subpass
        Dependency.srcStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        Dependency.srcAccessMask = 0;
        Dependency.dstStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        Dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        std::array<VkAttachmentDescription, 1> AttachmentDescriptions = {ColorAttachment};

        VkRenderPassCreateInfo RenderPassInfo{};
        RenderPassInfo.sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        RenderPassInfo.attachmentCount = static_cast<UInt32>(AttachmentDescriptions.size());
        RenderPassInfo.pAttachments    = AttachmentDescriptions.data();
        RenderPassInfo.subpassCount    = 1;
        RenderPassInfo.pSubpasses      = &Subpass;
        RenderPassInfo.dependencyCount = 1;
        RenderPassInfo.pDependencies   = &Dependency;

        if (vkCreateRenderPass(m_Device, &RenderPassInfo, nullptr, &m_GUIRenderPass) != VK_SUCCESS)
        {
            CORVUS_CORE_CRITICAL("Failed to create GUI Vulkan Render Pass!");
        }
        CORVUS_CORE_TRACE("Created GUI Vulkan Render Pass successfully");
    }

    void CRenderer::DestroyGUIRenderPass()
    {
        if (m_GUIRenderPass)
        {
            vkDestroyRenderPass(m_Device, m_GUIRenderPass, nullptr);
            m_GUIRenderPass = VK_NULL_HANDLE;
            CORVUS_CORE_TRACE("Vulkan GUI Render Pass destroyed");
        }
    }

} // namespace Corvus
