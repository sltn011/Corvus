#include "CorvusPCH.h"

#include "Corvus/Renderer/RenderPass/RenderPass_Combine.h"

#include "Corvus/Renderer/Data/PushConstants.h"
#include "Corvus/Renderer/Data/ScreenQuad.h"
#include "Corvus/Renderer/Data/UBOs.h"
#include "Corvus/Renderer/Data/Vertex.h"
#include "Corvus/Renderer/Renderer.h"

namespace Corvus
{

    void CRenderPass_Combine::Create()
    {
        CreateRenderPass();
        CreateLayout();
        CreatePipeline();
    }

    void CRenderPass_Combine::Destroy()
    {
        vkDestroyPipeline(Renderer().Device, Pipeline, nullptr);
        vkDestroyPipelineLayout(Renderer().Device, PipelineLayout, nullptr);
        vkDestroyRenderPass(Renderer().Device, RenderPass, nullptr);

        Pipeline       = VK_NULL_HANDLE;
        PipelineLayout = VK_NULL_HANDLE;
        RenderPass     = VK_NULL_HANDLE;
    }

    void CRenderPass_Combine::Render(VkCommandBuffer CommandBuffer, UInt32 SwapchainImageIndex)
    {
        std::array<VkClearValue, 1> ClearColors;
        ClearColors[0].color = VkClearColorValue{0.0f, 0.0f, 0.0f, 1.0f};

        VkRenderPassBeginInfo RenderPassBeginInfo = VkInit::RenderPassBeginInfo(
            RenderPass,
            Renderer().SwapchainFramebuffers[SwapchainImageIndex],
            Renderer().SwapchainExtent,
            ClearColors.data(),
            ClearColors.size()
        );

        vkCmdBeginRenderPass(CommandBuffer, &RenderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(CommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, Pipeline);

        // Viewport and Scissor are dynamic - specify them here
        VkViewport Viewport = VkInit::Viewport(Renderer().SwapchainExtent);
        vkCmdSetViewport(CommandBuffer, 0, 1, &Viewport);

        VkRect2D Scissor = VkInit::Scissor(Renderer().SwapchainExtent);
        vkCmdSetScissor(CommandBuffer, 0, 1, &Scissor);

        vkCmdBindDescriptorSets(
            CommandBuffer,
            VK_PIPELINE_BIND_POINT_GRAPHICS,
            PipelineLayout,
            0,
            1,
            &Renderer().RenderPass_Deferred.RenderTarget.DescriptorSet,
            0,
            nullptr
        );

        VkBuffer     Buffers[] = {CScreenQuad::Get().VertexBuffer.Buffer};
        VkDeviceSize Offsets[] = {0};
        vkCmdBindVertexBuffers(CommandBuffer, 0, 1, Buffers, Offsets);

        vkCmdBindIndexBuffer(CommandBuffer, CScreenQuad::Get().IndexBuffer.Buffer, 0, VK_INDEX_TYPE_UINT16);

        vkCmdDrawIndexed(CommandBuffer, 6, 1, 0, 0, 0);

        vkCmdEndRenderPass(CommandBuffer);
    }

    void CRenderPass_Combine::CreateRenderPass()
    {
        VkAttachmentDescription ColorAttachment = VkInit::AttachmentDescription(
            Renderer().SwapchainImageFormat,
            VK_IMAGE_LAYOUT_UNDEFINED,
            VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
            VK_ATTACHMENT_LOAD_OP_CLEAR,
            VK_ATTACHMENT_STORE_OP_STORE,
            VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            VK_ATTACHMENT_STORE_OP_DONT_CARE
        );

        VkAttachmentReference ColorAttachmentRef =
            VkInit::AttachmentReference(0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);

        VkSubpassDescription Subpass =
            VkInit::SubpassDescription(VK_PIPELINE_BIND_POINT_GRAPHICS, &ColorAttachmentRef, 1, nullptr);

        VkSubpassDependency Dependency = VkInit::SubpassDependency(
            VK_SUBPASS_EXTERNAL,
            0,
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
            0,
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
            VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT
        );

        std::array<VkAttachmentDescription, 1> AttachmentDescriptions = {ColorAttachment};

        VkRenderPassCreateInfo RenderPassInfo = VkInit::RenderPassCreateInfo(
            AttachmentDescriptions.data(), AttachmentDescriptions.size(), &Subpass, 1, &Dependency, 1
        );

        if (vkCreateRenderPass(Renderer().Device, &RenderPassInfo, nullptr, &RenderPass) != VK_SUCCESS)
        {
            CORVUS_CORE_CRITICAL("Failed to create Combine Render Pass!");
        }
        CORVUS_CORE_TRACE("Created Combine Render Pass successfully");
    }

    void CRenderPass_Combine::CreateLayout()
    {
        std::vector<VkDescriptorSetLayout> SetLayouts = {Renderer().PerDrawDescriptorSetLayout};

        VkPipelineLayoutCreateInfo PipelineLayoutInfo =
            VkInit::PipelineLayoutCreateInfo(SetLayouts.data(), SetLayouts.size(), nullptr, 0);

        if (vkCreatePipelineLayout(Renderer().Device, &PipelineLayoutInfo, nullptr, &PipelineLayout) != VK_SUCCESS)
        {
            CORVUS_CORE_CRITICAL("Failed to create Combine Pipeline Layout!");
        }
        CORVUS_CORE_TRACE("Created Combine Pipeline Layout successfully");
    }

    void CRenderPass_Combine::CreatePipeline()
    {
        // Programmable stages ===============================================================================
        std::vector<char> VertexShaderByteCode   = Renderer().ReadSPIRVByteCode("./Assets/Shaders/Combine.vert.spv");
        std::vector<char> FragmentShaderByteCode = Renderer().ReadSPIRVByteCode("./Assets/Shaders/Combine.frag.spv");

        VkShaderModule VertexShaderModule   = Renderer().CreateShaderModule(VertexShaderByteCode);
        VkShaderModule FragmentShaderModule = Renderer().CreateShaderModule(FragmentShaderByteCode);

        VkPipelineShaderStageCreateInfo VertexShaderStageInfo =
            VkInit::PipelineShaderStageCreateInfo(VertexShaderModule, VK_SHADER_STAGE_VERTEX_BIT);

        VkPipelineShaderStageCreateInfo FragmentShaderStageInfo =
            VkInit::PipelineShaderStageCreateInfo(FragmentShaderModule, VK_SHADER_STAGE_FRAGMENT_BIT);

        std::vector<VkPipelineShaderStageCreateInfo> ShaderStagesInfo = {
            VertexShaderStageInfo, FragmentShaderStageInfo};
        // ===================================================================================================

        // Fixed stages ======================================================================================
        std::vector<VkVertexInputBindingDescription> VertexInputBindingDescriptions = {
            CQuadVertex::GetInputBindingDescription()};
        std::vector<VkVertexInputAttributeDescription> VertexInputAttributeDescriptions =
            CQuadVertex::GetInputAttributeDescriptions();

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
        VkPipelineRasterizationStateCreateInfo RasterizerStateInfo =
            VkInit::PipelineRasterizationStateCreateInfo(false);
        // Frontface swapped bcz of flipped Y in proj
        // ===================================================================================================

        // Multisampling =====================================================================================
        VkPipelineMultisampleStateCreateInfo MultisamplerStateInfo = VkInit::PipelineMultisampleStateCreateInfo();
        // ===================================================================================================

        // Depth and Stencil testing =========================================================================
        VkPipelineDepthStencilStateCreateInfo DepthStencilTestStateInfo =
            VkInit::PipelineDepthStencilStateCreateInfo(false, false);
        // ===================================================================================================

        // Color Blending ====================================================================================
        std::vector<VkPipelineColorBlendAttachmentState> ColorBlendAttachments = {
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
            PipelineLayout,
            RenderPass
        );

        VkResult PipelineCreateResult =
            vkCreateGraphicsPipelines(Renderer().Device, VK_NULL_HANDLE, 1, &PipelineCreateInfo, nullptr, &Pipeline);

        Renderer().DestroyShaderModule(FragmentShaderModule);
        Renderer().DestroyShaderModule(VertexShaderModule);

        if (PipelineCreateResult != VK_SUCCESS)
        {
            CORVUS_CORE_CRITICAL("Failed to create Combine Pipeline!");
        }
        CORVUS_CORE_TRACE("Created Combine Pipeline successfully");
    }

} // namespace Corvus
