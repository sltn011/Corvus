#include "CorvusPCH.h"

#include "Corvus/Renderer/Data/PushConstants.h"
#include "Corvus/Renderer/Renderer.h"

namespace Corvus
{

    void CRenderer::CreatePipelineLayout()
    {
        VkPushConstantRange PushConstantRange{};
        PushConstantRange.offset     = 0;
        PushConstantRange.size       = sizeof(CModelPushConstant);
        PushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

        std::array<VkDescriptorSetLayout, 2> SetLayouts = {PerFrameDescriptorSetLayout, PerDrawDescriptorSetLayout};

        VkPipelineLayoutCreateInfo PipelineLayoutInfo{};
        PipelineLayoutInfo.sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        PipelineLayoutInfo.setLayoutCount         = static_cast<UInt32>(SetLayouts.size());
        PipelineLayoutInfo.pSetLayouts            = SetLayouts.data();
        PipelineLayoutInfo.pushConstantRangeCount = 1;
        PipelineLayoutInfo.pPushConstantRanges    = &PushConstantRange;

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
