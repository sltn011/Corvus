#include "CorvusPCH.h"

#include "Corvus/Renderer/RenderPass/RenderPass_Deferred.h"

#include "Corvus/Renderer/Data/PushConstants.h"
#include "Corvus/Renderer/Data/ScreenQuad.h"
#include "Corvus/Renderer/Data/UBOs.h"
#include "Corvus/Renderer/Data/Vertex.h"
#include "Corvus/Renderer/Renderer.h"

namespace Corvus
{

    void CRenderPass_Deferred::Create()
    {
        CreateRenderPass();
        CreateLayout();
        CreatePipeline();
        CreateRenderTarget();
    }

    void CRenderPass_Deferred::Destroy()
    {
        Renderer().DestroyRenderTarget(RenderTarget);
        vkDestroyPipeline(Renderer().Device, Pipeline, nullptr);
        vkDestroyPipelineLayout(Renderer().Device, PipelineLayout, nullptr);
        vkDestroyRenderPass(Renderer().Device, RenderPass, nullptr);

        Pipeline       = VK_NULL_HANDLE;
        PipelineLayout = VK_NULL_HANDLE;
        RenderPass     = VK_NULL_HANDLE;
    }

    void CRenderPass_Deferred::BeginRender(VkCommandBuffer CommandBuffer)
    {
        std::array<VkClearValue, 4> ClearColors;
        ClearColors[0].color        = VkClearColorValue{0.f, 0.f, 0.f, 1.0f};
        ClearColors[1].color        = VkClearColorValue{0.f, 0.f, 0.f, 1.0f};
        ClearColors[2].color        = VkClearColorValue{0.f, 0.f, 0.f, 1.0f};
        ClearColors[3].depthStencil = VkClearDepthStencilValue{1.0f, 0};

        VkRenderPassBeginInfo RenderPassBeginInfo{};
        RenderPassBeginInfo.sType             = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        RenderPassBeginInfo.renderPass        = RenderPass;
        RenderPassBeginInfo.framebuffer       = RenderTarget.Framebuffer;
        RenderPassBeginInfo.renderArea.offset = {0, 0};
        RenderPassBeginInfo.renderArea.extent = RenderTarget.Extent;
        RenderPassBeginInfo.clearValueCount   = static_cast<UInt32>(ClearColors.size());
        RenderPassBeginInfo.pClearValues      = ClearColors.data();

        vkCmdBeginRenderPass(CommandBuffer, &RenderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(CommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, Pipeline);

        // Viewport and Scissor are dynamic - specify them here
        VkViewport Viewport{};
        Viewport.x        = 0.0f;
        Viewport.y        = 0.0f;
        Viewport.width    = static_cast<float>(RenderTarget.Extent.width);
        Viewport.height   = static_cast<float>(RenderTarget.Extent.height);
        Viewport.minDepth = 0.0f;
        Viewport.maxDepth = 1.0f;
        vkCmdSetViewport(CommandBuffer, 0, 1, &Viewport);

        VkRect2D Scissor{};
        Scissor.offset = {0, 0};
        Scissor.extent = RenderTarget.Extent;
        vkCmdSetScissor(CommandBuffer, 0, 1, &Scissor);

        vkCmdBindDescriptorSets(
            CommandBuffer,
            VK_PIPELINE_BIND_POINT_GRAPHICS,
            PipelineLayout,
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
            ColorAttachments[0].format         = VK_FORMAT_R16G16B16A16_SFLOAT;
            ColorAttachments[0].samples        = VK_SAMPLE_COUNT_1_BIT;
            ColorAttachments[0].loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
            ColorAttachments[0].storeOp        = VK_ATTACHMENT_STORE_OP_STORE;
            ColorAttachments[0].stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            ColorAttachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            ColorAttachments[0].initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
            ColorAttachments[0].finalLayout    = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

            ColorAttachmentRefs[0].attachment = 0; // index of attachment in pAttachments array in RenderPassInfo
            ColorAttachmentRefs[0].layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        }

        // Albedo
        {
            ColorAttachments[1].format         = VK_FORMAT_R8G8B8A8_UNORM;
            ColorAttachments[1].samples        = VK_SAMPLE_COUNT_1_BIT;
            ColorAttachments[1].loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
            ColorAttachments[1].storeOp        = VK_ATTACHMENT_STORE_OP_STORE;
            ColorAttachments[1].stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            ColorAttachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            ColorAttachments[1].initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
            ColorAttachments[1].finalLayout    = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

            ColorAttachmentRefs[1].attachment = 1;
            ColorAttachmentRefs[1].layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        }

        // Normal
        {
            ColorAttachments[2].format         = VK_FORMAT_R16G16B16A16_SFLOAT;
            ColorAttachments[2].samples        = VK_SAMPLE_COUNT_1_BIT;
            ColorAttachments[2].loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
            ColorAttachments[2].storeOp        = VK_ATTACHMENT_STORE_OP_STORE;
            ColorAttachments[2].stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            ColorAttachments[2].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            ColorAttachments[2].initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
            ColorAttachments[2].finalLayout    = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

            ColorAttachmentRefs[2].attachment = 2;
            ColorAttachmentRefs[2].layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        }

        // Depth
        {
            VkFormat DepthFormat = Renderer().FindSupportedFormat(
                {VK_FORMAT_D24_UNORM_S8_UINT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D32_SFLOAT},
                VK_IMAGE_TILING_OPTIMAL,
                VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
            );

            DepthAttachment.format         = DepthFormat;
            DepthAttachment.samples        = VK_SAMPLE_COUNT_1_BIT;
            DepthAttachment.loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
            DepthAttachment.storeOp        = VK_ATTACHMENT_STORE_OP_STORE;
            DepthAttachment.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            DepthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            DepthAttachment.initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
            DepthAttachment.finalLayout    = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

            DepthAttachmentRef.attachment = 3;
            DepthAttachmentRef.layout     = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        }

        VkSubpassDescription Subpass{};
        Subpass.pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS;
        Subpass.colorAttachmentCount    = static_cast<UInt32>(ColorAttachmentRefs.size());
        Subpass.pColorAttachments       = ColorAttachmentRefs.data();
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

        std::array<VkAttachmentDescription, 4> AttachmentDescriptions = {
            ColorAttachments[0], ColorAttachments[1], ColorAttachments[2], DepthAttachment};

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
            CORVUS_CORE_CRITICAL("Failed to create Deferred Render Pass!");
        }
        CORVUS_CORE_TRACE("Created Deferred Render Pass successfully");
    }

    void CRenderPass_Deferred::CreateLayout()
    {
        VkPushConstantRange PushConstantRange{};
        PushConstantRange.offset     = 0;
        PushConstantRange.size       = sizeof(CModelPushConstant);
        PushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

        std::array<VkDescriptorSetLayout, 2> SetLayouts = {
            Renderer().PerFrameDescriptorSetLayout, Renderer().PerDrawDescriptorSetLayout};

        VkPipelineLayoutCreateInfo PipelineLayoutInfo{};
        PipelineLayoutInfo.sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        PipelineLayoutInfo.setLayoutCount         = static_cast<UInt32>(SetLayouts.size());
        PipelineLayoutInfo.pSetLayouts            = SetLayouts.data();
        PipelineLayoutInfo.pushConstantRangeCount = 1;
        PipelineLayoutInfo.pPushConstantRanges    = &PushConstantRange;

        if (vkCreatePipelineLayout(Renderer().Device, &PipelineLayoutInfo, nullptr, &PipelineLayout) != VK_SUCCESS)
        {
            CORVUS_CORE_CRITICAL("Failed to create Deferred Pipeline Layout!");
        }
        CORVUS_CORE_TRACE("Created Deferred Pipeline Layout successfully");
    }

    void CRenderPass_Deferred::CreatePipeline()
    {
        // Programmable stages ===============================================================================
        std::vector<char> VertexShaderByteCode   = Renderer().ReadSPIRVByteCode("./Assets/Shaders/Deferred.vert.spv");
        std::vector<char> FragmentShaderByteCode = Renderer().ReadSPIRVByteCode("./Assets/Shaders/Deferred.frag.spv");

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
        VkVertexInputBindingDescription      VertexInputBindingDescription = CVertex::GetInputBindingDescription();
        std::array<VkVertexInputAttributeDescription, 3> VertexInputAttributeDescriptions =
            CVertex::GetInputAttributeDescriptions();

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
        RasterizerStageInfo.cullMode                = VK_CULL_MODE_BACK_BIT;
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
        DepthStencilTestStateInfo.depthTestEnable       = VK_TRUE;
        DepthStencilTestStateInfo.depthWriteEnable      = VK_TRUE;
        DepthStencilTestStateInfo.depthCompareOp        = VK_COMPARE_OP_LESS;
        DepthStencilTestStateInfo.depthBoundsTestEnable = VK_FALSE;
        DepthStencilTestStateInfo.minDepthBounds        = 0.0f;
        DepthStencilTestStateInfo.maxDepthBounds        = 1.0f;
        DepthStencilTestStateInfo.stencilTestEnable     = VK_FALSE;
        DepthStencilTestStateInfo.front                 = {};
        DepthStencilTestStateInfo.back                  = {};
        // ===================================================================================================

        // Color Blending ====================================================================================
        std::array<VkPipelineColorBlendAttachmentState, 3> ColorBlendAttachments{};
        for (SizeT i = 0; i < ColorBlendAttachments.size(); ++i)
        {
            ColorBlendAttachments[i].colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
                                                      VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
            ColorBlendAttachments[i].blendEnable         = VK_FALSE;
            ColorBlendAttachments[i].srcColorBlendFactor = VK_BLEND_FACTOR_ONE;  // optional
            ColorBlendAttachments[i].dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // optional
            ColorBlendAttachments[i].colorBlendOp        = VK_BLEND_OP_ADD;      // optional
            ColorBlendAttachments[i].srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;  // optional
            ColorBlendAttachments[i].dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // optional
            ColorBlendAttachments[i].alphaBlendOp        = VK_BLEND_OP_ADD;      // optional
        }

        VkPipelineColorBlendStateCreateInfo ColorBlendState{};
        ColorBlendState.sType             = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        ColorBlendState.logicOpEnable     = VK_FALSE;
        ColorBlendState.logicOp           = VK_LOGIC_OP_COPY; // optional
        ColorBlendState.attachmentCount   = static_cast<UInt32>(ColorBlendAttachments.size());
        ColorBlendState.pAttachments      = ColorBlendAttachments.data();
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
            CORVUS_CORE_CRITICAL("Failed to create Deferred Pipeline!");
        }
        CORVUS_CORE_TRACE("Created Deferred Pipeline successfully");
    }

    void CRenderPass_Deferred::CreateRenderTarget()
    {
        VkFormat DepthFormat = Renderer().FindSupportedFormat(
            {VK_FORMAT_D24_UNORM_S8_UINT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D32_SFLOAT},
            VK_IMAGE_TILING_OPTIMAL,
            VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
        );

        std::vector<CAttachment> Attachments(4);
        Attachments[0] = Renderer().CreateColorAttachment(VK_FORMAT_R16G16B16A16_SFLOAT, Renderer().SwapchainExtent);
        Attachments[1] = Renderer().CreateColorAttachment(VK_FORMAT_R8G8B8A8_UNORM, Renderer().SwapchainExtent);
        Attachments[2] = Renderer().CreateColorAttachment(VK_FORMAT_R16G16B16A16_SFLOAT, Renderer().SwapchainExtent);
        Attachments[3] = Renderer().CreateDepthStencilAttachment(DepthFormat, Renderer().SwapchainExtent);

        RenderTarget = Renderer().CreateRenderTarget(
            std::move(Attachments), Renderer().SwapchainExtent, RenderPass, Renderer().PerDrawDescriptorSetLayout
        );
    }

} // namespace Corvus
