#include "CorvusPCH.h"

#include "Corvus/Renderer/Data/PushConstants.h"
#include "Corvus/Renderer/Renderer.h"

namespace Corvus
{

    void CRenderer::CreatePipelineLayout()
    {
        std::vector<VkDescriptorSetLayout> DescriptorSetsLayouts = {
            PerFrameDescriptorSetLayout, PerDrawDescriptorSetLayout};

        std::vector<VkPushConstantRange> PushConstantsRanges = {
            VkInit::PushConstantRange(0, sizeof(CModelPushConstant), VK_SHADER_STAGE_VERTEX_BIT)};

        VkPipelineLayoutCreateInfo PipelineLayoutInfo = VkInit::PipelineLayoutCreateInfo(
            DescriptorSetsLayouts.data(),
            DescriptorSetsLayouts.size(),
            PushConstantsRanges.data(),
            PushConstantsRanges.size()
        );

        if (vkCreatePipelineLayout(Device, &PipelineLayoutInfo, nullptr, &PipelineLayout) != VK_SUCCESS)
        {
            CORVUS_CORE_CRITICAL("Failed to create Vulkan Pipeline Layout!");
        }
        CORVUS_CORE_TRACE("Created Vulkan Pipeline Layout successfully");
    }

    void CRenderer::DestroyPipelineLayout()
    {
        if (PipelineLayout)
        {
            vkDestroyPipelineLayout(Device, PipelineLayout, nullptr);
            PipelineLayout = VK_NULL_HANDLE;
            CORVUS_CORE_TRACE("Vulkan Pipeline Layout destroyed");
        }
    }

} // namespace Corvus
