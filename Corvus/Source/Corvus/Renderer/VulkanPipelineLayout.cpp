#include "CorvusPCH.h"

#include "Corvus/Renderer/Renderer.h"

namespace Corvus
{

    void CRenderer::CreatePipelineLayout()
    {
        VkPipelineLayoutCreateInfo PipelineLayoutInfo{};
        PipelineLayoutInfo.sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        PipelineLayoutInfo.setLayoutCount         = 1;
        PipelineLayoutInfo.pSetLayouts            = &m_MatricesUBOLayout;
        PipelineLayoutInfo.pushConstantRangeCount = 0;       // optional
        PipelineLayoutInfo.pPushConstantRanges    = nullptr; // optional

        if (vkCreatePipelineLayout(m_Device, &PipelineLayoutInfo, nullptr, &m_PipelineLayout) != VK_SUCCESS)
        {
            CORVUS_CORE_CRITICAL("Failed to create Vulkan Pipeline Layout!");
        }
        CORVUS_CORE_TRACE("Created Vulkan Pipeline Layout successfully");
    }

    void CRenderer::DestroyPipelineLayout()
    {
        if (m_PipelineLayout)
        {
            vkDestroyPipelineLayout(m_Device, m_PipelineLayout, nullptr);
            m_PipelineLayout = VK_NULL_HANDLE;
            CORVUS_CORE_TRACE("Vulkan Pipeline Layout destroyed");
        }
    }

} // namespace Corvus
