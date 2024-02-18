#include "CorvusPCH.h"

#include "Corvus/Renderer/RenderPass/RenderPass_Deferred.h"

#include "Corvus/Renderer/Data/PushConstants.h"
#include "Corvus/Renderer/Data/UBOs.h"
#include "Corvus/Renderer/Data/Vertex.h"
#include "Corvus/Renderer/Renderer.h"

namespace Corvus
{

    void CRenderPass_Deferred::Create()
    {
        CreateRenderPass();
        CreatePipeline();
        CreateRenderTarget();
    }

    void CRenderPass_Deferred::Destroy()
    {
        Renderer().DestroyRenderTarget(RenderTarget);
        vkDestroyPipeline(Renderer().Device, Pipeline, nullptr);
        vkDestroyRenderPass(Renderer().Device, RenderPass, nullptr);

        Pipeline   = VK_NULL_HANDLE;
        RenderPass = VK_NULL_HANDLE;
    }

    void CRenderPass_Deferred::BeginRender(VkCommandBuffer CommandBuffer)
    {
        std::array<VkClearValue, 4> ClearColors;
        ClearColors[0].color        = VkClearColorValue{0.f, 0.f, 0.f, 1.0f};
        ClearColors[1].color        = VkClearColorValue{1.f, 1.f, 1.f, 1.0f};
        ClearColors[2].color        = VkClearColorValue{0.f, 0.f, 0.f, 1.0f};
        ClearColors[3].depthStencil = VkClearDepthStencilValue{1.0f, 0};

        VkRenderPassBeginInfo RenderPassBeginInfo = VkInit::RenderPassBeginInfo(
            RenderPass, RenderTarget.Framebuffer, RenderTarget.Extent, ClearColors.data(), ClearColors.size()
        );

        vkCmdBeginRenderPass(CommandBuffer, &RenderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(CommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, Pipeline);

        // Viewport and Scissor are dynamic - specify them here
        VkViewport Viewport = VkInit::Viewport(RenderTarget.Extent);
        vkCmdSetViewport(CommandBuffer, 0, 1, &Viewport);

        VkRect2D Scissor = VkInit::Scissor(RenderTarget.Extent);
        vkCmdSetScissor(CommandBuffer, 0, 1, &Scissor);

        vkCmdBindDescriptorSets(
            CommandBuffer,
            VK_PIPELINE_BIND_POINT_GRAPHICS,
            Renderer().PipelineLayout,
            0,
            1,
            &Renderer().PerFrameDescriptorSets[Renderer().CurrentFrame()],
            0,
            nullptr
        );
    }

    void CRenderPass_Deferred::EndRender(VkCommandBuffer CommandBuffer)
    {
        vkCmdEndRenderPass(CommandBuffer);
    }

    void CRenderPass_Deferred::CreateRenderPass()
    {
        std::array<VkAttachmentDescription, 3> ColorAttachments{};
        std::array<VkAttachmentReference, 3>   ColorAttachmentRefs{};

        VkAttachmentDescription DepthAttachment{};
        VkAttachmentReference   DepthAttachmentRef{};

        // Position
        {
            ColorAttachments[0] = VkInit::AttachmentDescription(
                VK_FORMAT_R16G16B16A16_SFLOAT,
                VK_IMAGE_LAYOUT_UNDEFINED,
                VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                VK_ATTACHMENT_LOAD_OP_CLEAR,
                VK_ATTACHMENT_STORE_OP_STORE,
                VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                VK_ATTACHMENT_STORE_OP_DONT_CARE
            );

            AttachmentsDescriptions.push_back(ColorAttachments[0]);

            ColorAttachmentRefs[0] = VkInit::AttachmentReference(0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
        }

        // Albedo
        {
            ColorAttachments[1] = VkInit::AttachmentDescription(
                VK_FORMAT_R8G8B8A8_UNORM,
                VK_IMAGE_LAYOUT_UNDEFINED,
                VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                VK_ATTACHMENT_LOAD_OP_CLEAR,
                VK_ATTACHMENT_STORE_OP_STORE,
                VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                VK_ATTACHMENT_STORE_OP_DONT_CARE
            );

            AttachmentsDescriptions.push_back(ColorAttachments[1]);

            ColorAttachmentRefs[1] = VkInit::AttachmentReference(1, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
        }

        // Normal
        {
            ColorAttachments[2] = VkInit::AttachmentDescription(
                VK_FORMAT_R16G16B16A16_SFLOAT,
                VK_IMAGE_LAYOUT_UNDEFINED,
                VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                VK_ATTACHMENT_LOAD_OP_CLEAR,
                VK_ATTACHMENT_STORE_OP_STORE,
                VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                VK_ATTACHMENT_STORE_OP_DONT_CARE
            );

            AttachmentsDescriptions.push_back(ColorAttachments[2]);

            ColorAttachmentRefs[2] = VkInit::AttachmentReference(2, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
        }

        // Depth
        {
            VkFormat DepthFormat = Renderer().FindSupportedFormat(
                {VK_FORMAT_D24_UNORM_S8_UINT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D32_SFLOAT},
                VK_IMAGE_TILING_OPTIMAL,
                VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
            );

            DepthAttachment = VkInit::AttachmentDescription(
                DepthFormat,
                VK_IMAGE_LAYOUT_UNDEFINED,
                VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                VK_ATTACHMENT_LOAD_OP_CLEAR,
                VK_ATTACHMENT_STORE_OP_STORE,
                VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                VK_ATTACHMENT_STORE_OP_DONT_CARE
            );

            AttachmentsDescriptions.push_back(DepthAttachment);

            DepthAttachmentRef = VkInit::AttachmentReference(3, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
        }

        VkSubpassDescription Subpass = VkInit::SubpassDescription(
            VK_PIPELINE_BIND_POINT_GRAPHICS, ColorAttachmentRefs.data(), ColorAttachmentRefs.size(), &DepthAttachmentRef
        );

        VkSubpassDependency Dependency = VkInit::SubpassDependency(
            VK_SUBPASS_EXTERNAL,
            0,
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
            0,
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
            VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT
        );

        std::array<VkAttachmentDescription, 4> AttachmentDescriptions = {
            ColorAttachments[0], ColorAttachments[1], ColorAttachments[2], DepthAttachment};

        VkRenderPassCreateInfo RenderPassInfo = VkInit::RenderPassCreateInfo(
            AttachmentDescriptions.data(), AttachmentDescriptions.size(), &Subpass, 1, &Dependency, 1
        );

        if (vkCreateRenderPass(Renderer().Device, &RenderPassInfo, nullptr, &RenderPass) != VK_SUCCESS)
        {
            CORVUS_CORE_CRITICAL("Failed to create Deferred Render Pass!");
        }
        CORVUS_CORE_TRACE("Created Deferred Render Pass successfully");
    }

    void CRenderPass_Deferred::CreatePipeline()
    {
        // Programmable stages ===============================================================================
        std::vector<char> VertexShaderByteCode   = Renderer().ReadSPIRVByteCode("./Assets/Shaders/Deferred.vert.spv");
        std::vector<char> FragmentShaderByteCode = Renderer().ReadSPIRVByteCode("./Assets/Shaders/Deferred.frag.spv");

        VkShaderModule VertexShaderModule   = Renderer().CreateShaderModule(VertexShaderByteCode);
        VkShaderModule FragmentShaderModule = Renderer().CreateShaderModule(FragmentShaderByteCode);

        VkPipelineShaderStageCreateInfo VertexShaderStageInfo =
            VkInit::PipelineShaderStageCreateInfo(VertexShaderModule, VK_SHADER_STAGE_VERTEX_BIT);

        VkPipelineShaderStageCreateInfo FragmentShaderStageInfo =
            VkInit::PipelineShaderStageCreateInfo(FragmentShaderModule, VK_SHADER_STAGE_FRAGMENT_BIT);

        std::vector<VkPipelineShaderStageCreateInfo> ShaderStagesInfo = {
            VertexShaderStageInfo, FragmentShaderStageInfo};
        // ===================================================================================================

        // Vertex Input ======================================================================================
        std::vector<VkVertexInputBindingDescription> VertexInputBindingDescriptions = {
            CVertex::GetInputBindingDescription()};
        std::vector<VkVertexInputAttributeDescription> VertexInputAttributeDescriptions =
            CVertex::GetInputAttributeDescriptions();

        VkPipelineVertexInputStateCreateInfo VertexInputStateInfo = VkInit::PipelineVertexInputStateInfo(
            VertexInputBindingDescriptions.data(),
            VertexInputBindingDescriptions.size(),
            VertexInputAttributeDescriptions.data(),
            VertexInputAttributeDescriptions.size()
        );

        VkPipelineInputAssemblyStateCreateInfo InputAssemblyStateInfo = VkInit::PipelineInputAssemblyStateCreateInfo();
        // ===================================================================================================

        // Dynamic Viewport and Scissor ======================================================================
        std::vector<VkDynamicState>      DynamicStates = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
        VkPipelineDynamicStateCreateInfo DynamicStateInfo =
            VkInit::PipelineDynamicStateCreateInfo(DynamicStates.data(), DynamicStates.size());

        VkPipelineViewportStateCreateInfo ViewportState = VkInit::PipelineViewportStateCreateInfo();
        // ===================================================================================================

        // Rasterizer ========================================================================================
        VkPipelineRasterizationStateCreateInfo RasterizerStateInfo = VkInit::PipelineRasterizationStateCreateInfo(true);
        // Frontface swapped bcz of flipped Y in proj
        // ===================================================================================================

        // Multisampling =====================================================================================
        VkPipelineMultisampleStateCreateInfo MultisamplerStateInfo = VkInit::PipelineMultisampleStateCreateInfo();
        // ===================================================================================================

        // Depth and Stencil testing =========================================================================
        VkPipelineDepthStencilStateCreateInfo DepthStencilTestStateInfo =
            VkInit::PipelineDepthStencilStateCreateInfo(true, true);
        // ===================================================================================================

        // Color Blending ====================================================================================
        std::vector<VkPipelineColorBlendAttachmentState> ColorBlendAttachments = {
            VkInit::PipelineColorBlendAttachmentState(),
            VkInit::PipelineColorBlendAttachmentState(),
            VkInit::PipelineColorBlendAttachmentState()};

        VkPipelineColorBlendStateCreateInfo ColorBlendState =
            VkInit::PipelineColorBlendStateCreateInfo(ColorBlendAttachments.data(), ColorBlendAttachments.size());
        // ===================================================================================================

        // Pipeline creation =================================================================================
        VkGraphicsPipelineCreateInfo PipelineCreateInfo = VkInit::GraphicsPipelineCreateInfo(
            ShaderStagesInfo.data(),
            ShaderStagesInfo.size(),
            VertexInputStateInfo,
            InputAssemblyStateInfo,
            DynamicStateInfo,
            ViewportState,
            RasterizerStateInfo,
            MultisamplerStateInfo,
            DepthStencilTestStateInfo,
            ColorBlendState,
            Renderer().PipelineLayout,
            RenderPass
        );

        VkResult PipelineCreateResult =
            vkCreateGraphicsPipelines(Renderer().Device, VK_NULL_HANDLE, 1, &PipelineCreateInfo, nullptr, &Pipeline);

        Renderer().DestroyShaderModule(FragmentShaderModule);
        Renderer().DestroyShaderModule(VertexShaderModule);

        if (PipelineCreateResult != VK_SUCCESS)
        {
            CORVUS_CORE_CRITICAL("Failed to create Deferred Pipeline!");
        }
        CORVUS_CORE_TRACE("Created Deferred Pipeline successfully");
    }

    void CRenderPass_Deferred::CreateRenderTarget()
    {
        std::vector<CAttachment> Attachments(4);
        Attachments[0] = Renderer().CreateColorAttachment(
            AttachmentsDescriptions[0].format, Renderer().SwapchainExtent, AttachmentsDescriptions[0].finalLayout
        );
        Attachments[1] = Renderer().CreateColorAttachment(
            AttachmentsDescriptions[1].format, Renderer().SwapchainExtent, AttachmentsDescriptions[1].finalLayout
        );
        Attachments[2] = Renderer().CreateColorAttachment(
            AttachmentsDescriptions[2].format, Renderer().SwapchainExtent, AttachmentsDescriptions[2].finalLayout
        );
        Attachments[3] = Renderer().CreateDepthStencilAttachment(
            AttachmentsDescriptions[3].format, Renderer().SwapchainExtent, AttachmentsDescriptions[3].finalLayout
        );

        RenderTarget = Renderer().CreateRenderTarget(
            std::move(Attachments), Renderer().SwapchainExtent, RenderPass, Renderer().PerDrawDescriptorSetLayout
        );
    }

} // namespace Corvus
