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

        VkRenderPassBeginInfo RenderPassBeginInfo{};
        RenderPassBeginInfo.sType             = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        RenderPassBeginInfo.renderPass        = RenderPass;
        RenderPassBeginInfo.framebuffer       = Renderer().SwapchainFramebuffers[SwapchainImageIndex];
        RenderPassBeginInfo.renderArea.offset = {0, 0};
        RenderPassBeginInfo.renderArea.extent = Renderer().SwapchainExtent;

        RenderPassBeginInfo.clearValueCount = static_cast<UInt32>(ClearColors.size());
        RenderPassBeginInfo.pClearValues    = ClearColors.data();

        vkCmdBeginRenderPass(CommandBuffer, &RenderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(CommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, Pipeline);

        // Viewport and Scissor are dynamic - specify them here
        VkViewport Viewport{};
        Viewport.x        = 0.0f;
        Viewport.y        = 0.0f;
        Viewport.width    = static_cast<float>(Renderer().SwapchainExtent.width);
        Viewport.height   = static_cast<float>(Renderer().SwapchainExtent.height);
        Viewport.minDepth = 0.0f;
        Viewport.maxDepth = 1.0f;
        vkCmdSetViewport(CommandBuffer, 0, 1, &Viewport);

        VkRect2D Scissor{};
        Scissor.offset = {0, 0};
        Scissor.extent = Renderer().SwapchainExtent;
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
        VkAttachmentDescription ColorAttachment{};
        ColorAttachment.format         = Renderer().SwapchainImageFormat;
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
        Subpass.pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS;
        Subpass.colorAttachmentCount    = 1;
        Subpass.pColorAttachments       = &ColorAttachmentRef;
        Subpass.pDepthStencilAttachment = nullptr;

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

        if (vkCreateRenderPass(Renderer().Device, &RenderPassInfo, nullptr, &RenderPass) != VK_SUCCESS)
        {
            CORVUS_CORE_CRITICAL("Failed to create Combine Render Pass!");
        }
        CORVUS_CORE_TRACE("Created Combine Render Pass successfully");
    }

    void CRenderPass_Combine::CreateLayout()
    {
        std::array<VkDescriptorSetLayout, 1> SetLayouts = {Renderer().PerDrawDescriptorSetLayout};

        VkPipelineLayoutCreateInfo PipelineLayoutInfo{};
        PipelineLayoutInfo.sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        PipelineLayoutInfo.setLayoutCount         = static_cast<UInt32>(SetLayouts.size());
        PipelineLayoutInfo.pSetLayouts            = SetLayouts.data();
        PipelineLayoutInfo.pushConstantRangeCount = 0;
        PipelineLayoutInfo.pPushConstantRanges    = nullptr;

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

        VkPipelineShaderStageCreateInfo VertexShaderStageInfo{};
        VertexShaderStageInfo.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        VertexShaderStageInfo.stage  = VK_SHADER_STAGE_VERTEX_BIT;
        VertexShaderStageInfo.module = VertexShaderModule;
        VertexShaderStageInfo.pName  = "main";

        VkPipelineShaderStageCreateInfo FragmentShaderStageInfo{};
        FragmentShaderStageInfo.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        FragmentShaderStageInfo.stage  = VK_SHADER_STAGE_FRAGMENT_BIT;
        FragmentShaderStageInfo.module = FragmentShaderModule;
        FragmentShaderStageInfo.pName  = "main";

        VkPipelineShaderStageCreateInfo ShaderStagesInfo[] = {VertexShaderStageInfo, FragmentShaderStageInfo};
        // ===================================================================================================

        // Fixed stages ======================================================================================
        VkPipelineVertexInputStateCreateInfo VertexInputStageInfo{};
        VkVertexInputBindingDescription      VertexInputBindingDescription = CQuadVertex::GetInputBindingDescription();
        std::array<VkVertexInputAttributeDescription, 2> VertexInputAttributeDescriptions =
            CQuadVertex::GetInputAttributeDescriptions();

        VertexInputStageInfo.sType                         = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        VertexInputStageInfo.vertexBindingDescriptionCount = 1;
        VertexInputStageInfo.pVertexBindingDescriptions    = &VertexInputBindingDescription;
        VertexInputStageInfo.vertexAttributeDescriptionCount =
            static_cast<UInt32>(VertexInputAttributeDescriptions.size());
        VertexInputStageInfo.pVertexAttributeDescriptions = VertexInputAttributeDescriptions.data();

        VkPipelineInputAssemblyStateCreateInfo InputAssemblyStageInfo{};
        InputAssemblyStageInfo.sType                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        InputAssemblyStageInfo.topology               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        InputAssemblyStageInfo.primitiveRestartEnable = VK_FALSE;
        // ===================================================================================================

        // Dynamic Viewport and Scissor ======================================================================
        VkDynamicState                   DynamicStates[] = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
        VkPipelineDynamicStateCreateInfo DynamicStateInfo{};
        DynamicStateInfo.sType             = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        DynamicStateInfo.dynamicStateCount = sizeof(DynamicStates) / sizeof(DynamicStates[0]);
        DynamicStateInfo.pDynamicStates    = DynamicStates;

        VkPipelineViewportStateCreateInfo ViewportState{};
        ViewportState.sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        ViewportState.viewportCount = 1;
        ViewportState.pViewports    = nullptr; // ignored if dynamic
        ViewportState.scissorCount  = 1;
        ViewportState.pScissors     = nullptr; // ignored if dynamic
        // ===================================================================================================

        // Rasterizer ========================================================================================
        VkPipelineRasterizationStateCreateInfo RasterizerStageInfo{};
        RasterizerStageInfo.sType                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        RasterizerStageInfo.depthClampEnable        = VK_FALSE;
        RasterizerStageInfo.rasterizerDiscardEnable = VK_FALSE;
        RasterizerStageInfo.polygonMode             = VK_POLYGON_MODE_FILL;
        RasterizerStageInfo.lineWidth               = 1.0f;
        RasterizerStageInfo.cullMode                = VK_CULL_MODE_NONE;
        RasterizerStageInfo.frontFace               = VK_FRONT_FACE_COUNTER_CLOCKWISE;
        RasterizerStageInfo.depthBiasEnable         = VK_FALSE;
        RasterizerStageInfo.depthBiasConstantFactor = 0.0f; // optional
        RasterizerStageInfo.depthBiasClamp          = 0.0f; // optional
        RasterizerStageInfo.depthBiasSlopeFactor    = 0.0f; // optional
                                                            // Frontface swapped bcz of flipped Y in proj
        // ===================================================================================================

        // Multisampling =====================================================================================
        VkPipelineMultisampleStateCreateInfo MultisamplerStateInfo{};
        MultisamplerStateInfo.sType                 = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        MultisamplerStateInfo.sampleShadingEnable   = VK_FALSE;
        MultisamplerStateInfo.rasterizationSamples  = VK_SAMPLE_COUNT_1_BIT;
        MultisamplerStateInfo.minSampleShading      = 1.0f;     // optional
        MultisamplerStateInfo.pSampleMask           = nullptr;  // optional
        MultisamplerStateInfo.alphaToCoverageEnable = VK_FALSE; // optional
        MultisamplerStateInfo.alphaToOneEnable      = VK_FALSE; // optional
        // ===================================================================================================

        // Depth and Stencil testing =========================================================================
        VkPipelineDepthStencilStateCreateInfo DepthStencilTestStateInfo{};
        DepthStencilTestStateInfo.sType                 = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        DepthStencilTestStateInfo.depthTestEnable       = VK_FALSE;
        DepthStencilTestStateInfo.depthWriteEnable      = VK_FALSE;
        DepthStencilTestStateInfo.depthCompareOp        = VK_COMPARE_OP_LESS;
        DepthStencilTestStateInfo.depthBoundsTestEnable = VK_FALSE;
        DepthStencilTestStateInfo.minDepthBounds        = 0.0f;
        DepthStencilTestStateInfo.maxDepthBounds        = 1.0f;
        DepthStencilTestStateInfo.stencilTestEnable     = VK_FALSE;
        DepthStencilTestStateInfo.front                 = {};
        DepthStencilTestStateInfo.back                  = {};
        // ===================================================================================================

        // Color Blending ====================================================================================
        VkPipelineColorBlendAttachmentState ColorBlendAttachment{};
        ColorBlendAttachment.colorWriteMask =
            VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        ColorBlendAttachment.blendEnable         = VK_FALSE;
        ColorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;  // optional
        ColorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // optional
        ColorBlendAttachment.colorBlendOp        = VK_BLEND_OP_ADD;      // optional
        ColorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;  // optional
        ColorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // optional
        ColorBlendAttachment.alphaBlendOp        = VK_BLEND_OP_ADD;      // optional

        VkPipelineColorBlendStateCreateInfo ColorBlendState{};
        ColorBlendState.sType             = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        ColorBlendState.logicOpEnable     = VK_FALSE;
        ColorBlendState.logicOp           = VK_LOGIC_OP_COPY; // optional
        ColorBlendState.attachmentCount   = 1;
        ColorBlendState.pAttachments      = &ColorBlendAttachment;
        ColorBlendState.blendConstants[0] = 0.0f; // optional
        ColorBlendState.blendConstants[1] = 0.0f; // optional
        ColorBlendState.blendConstants[2] = 0.0f; // optional
        ColorBlendState.blendConstants[3] = 0.0f; // optional
        // ===================================================================================================

        // Pipeline creation =================================================================================
        VkGraphicsPipelineCreateInfo PipelineCreateInfo{};
        PipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;

        // Programmable stages
        PipelineCreateInfo.stageCount = 2;
        PipelineCreateInfo.pStages    = ShaderStagesInfo;

        // Fixed stages
        PipelineCreateInfo.pVertexInputState   = &VertexInputStageInfo;
        PipelineCreateInfo.pInputAssemblyState = &InputAssemblyStageInfo;
        PipelineCreateInfo.pDynamicState       = &DynamicStateInfo;
        PipelineCreateInfo.pViewportState      = &ViewportState;
        PipelineCreateInfo.pRasterizationState = &RasterizerStageInfo;
        PipelineCreateInfo.pMultisampleState   = &MultisamplerStateInfo;
        PipelineCreateInfo.pDepthStencilState  = &DepthStencilTestStateInfo;
        PipelineCreateInfo.pColorBlendState    = &ColorBlendState;

        // Uniforms and push-constants specified in layout
        PipelineCreateInfo.layout = PipelineLayout;

        // RenderPass and it's Subpass in which Pipeline is used
        PipelineCreateInfo.renderPass = RenderPass;
        PipelineCreateInfo.subpass    = 0;

        PipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
        PipelineCreateInfo.basePipelineIndex  = -1;

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
