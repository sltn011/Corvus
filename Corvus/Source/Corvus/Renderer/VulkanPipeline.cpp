#include "CorvusPCH.h"

#include "Corvus/Renderer/Data/Vertex.h"
#include "Corvus/Renderer/Renderer.h"

namespace Corvus
{

    void CRenderer::CreatePipeline()
    {
        CORVUS_ASSERT_FMT(m_Pipeline == VK_NULL_HANDLE, "Vulkan Pipeline was already created!");

        // Programmable stages ===============================================================================
        std::vector<char> VertexShaderByteCode   = ReadSPIRVByteCode("./Assets/Shaders/vert.spv");
        std::vector<char> FragmentShaderByteCode = ReadSPIRVByteCode("./Assets/Shaders/frag.spv");

        VkShaderModule VertexShaderModule   = CreateShaderModule(VertexShaderByteCode);
        VkShaderModule FragmentShaderModule = CreateShaderModule(FragmentShaderByteCode);

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
        std::array<VkVertexInputAttributeDescription, 2> VertexInputAttributeDescriptions =
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
        DepthStencilTestStateInfo.sType            = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        DepthStencilTestStateInfo.depthTestEnable  = true;
        DepthStencilTestStateInfo.depthWriteEnable = true;
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
        PipelineCreateInfo.layout = m_PipelineLayout;

        // RenderPass and it's Subpass in which Pipeline is used
        PipelineCreateInfo.renderPass = m_RenderPass;
        PipelineCreateInfo.subpass    = 0;

        PipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
        PipelineCreateInfo.basePipelineIndex  = -1;

        VkResult PipelineCreateResult =
            vkCreateGraphicsPipelines(m_Device, VK_NULL_HANDLE, 1, &PipelineCreateInfo, nullptr, &m_Pipeline);

        DestroyShaderModule(FragmentShaderModule);
        DestroyShaderModule(VertexShaderModule);

        if (PipelineCreateResult != VK_SUCCESS)
        {
            CORVUS_CORE_CRITICAL("Failed to create Vulkan Pipeline!");
        }
        CORVUS_CORE_TRACE("Created Vulkan Pipeline successfully");
    }

    void CRenderer::DestroyPipeline()
    {
        if (m_Pipeline)
        {
            vkDestroyPipeline(m_Device, m_Pipeline, nullptr);
            m_Pipeline = VK_NULL_HANDLE;
            CORVUS_CORE_TRACE("Vulkan Pipeline destroyed");
        }
    }

} // namespace Corvus
