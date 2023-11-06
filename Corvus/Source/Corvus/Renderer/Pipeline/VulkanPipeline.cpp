#include "CorvusPCH.h"

#include "Corvus/Renderer/Data/Vertex.h"
#include "Corvus/Renderer/Renderer.h"

namespace Corvus
{

    void CRenderer::CreatePipeline()
    {
        CORVUS_ASSERT_FMT(Pipeline == VK_NULL_HANDLE, "Vulkan Pipeline was already created!");

        // Programmable stages ===============================================================================
        std::vector<char> VertexShaderByteCode   = ReadSPIRVByteCode("./Assets/Shaders/vert.spv");
        std::vector<char> FragmentShaderByteCode = ReadSPIRVByteCode("./Assets/Shaders/frag.spv");

        VkShaderModule VertexShaderModule   = CreateShaderModule(VertexShaderByteCode);
        VkShaderModule FragmentShaderModule = CreateShaderModule(FragmentShaderByteCode);

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
        VkPipelineRasterizationStateCreateInfo RasterizerStateInfo = VkInit::PipelineRasterizationStateCreateInfo(true);
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
            vkCreateGraphicsPipelines(Device, VK_NULL_HANDLE, 1, &PipelineCreateInfo, nullptr, &Pipeline);

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
        if (Pipeline)
        {
            vkDestroyPipeline(Device, Pipeline, nullptr);
            Pipeline = VK_NULL_HANDLE;
            CORVUS_CORE_TRACE("Vulkan Pipeline destroyed");
        }
    }

} // namespace Corvus
