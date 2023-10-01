#include "CorvusPCH.h"

#include "Corvus/Renderer/Renderer.h"

namespace Corvus
{

    void CRenderer::AllocateDescriptorSets()
    {
        std::array<VkDescriptorSetLayout, s_FramesInFlight> SetsLayouts{};
        SetsLayouts.fill(m_MatricesUBOLayout);

        VkDescriptorSetAllocateInfo DescriptorSetInfo{};
        DescriptorSetInfo.sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        DescriptorSetInfo.descriptorPool     = m_DescriptorPool;
        DescriptorSetInfo.descriptorSetCount = static_cast<uint32_t>(s_FramesInFlight);
        DescriptorSetInfo.pSetLayouts        = SetsLayouts.data();

        if (vkAllocateDescriptorSets(m_Device, &DescriptorSetInfo, m_DescriptorSets.data()) != VK_SUCCESS)
        {
            CORVUS_CRITICAL("Failed to allocate Vulkan Descriptor Sets!");
        }
        CORVUS_TRACE("Allocated Vulkan Descriptor Sets successfully");

        // Allocated but not configured yet
        for (uint32_t i = 0; i < s_FramesInFlight; ++i)
        {
            VkDescriptorBufferInfo DescriptorBufferInfo{};
            DescriptorBufferInfo.buffer = m_MatricesUBOs[i].Buffer;
            DescriptorBufferInfo.offset = 0;
            DescriptorBufferInfo.range  = sizeof(CMVPUBO);

            VkWriteDescriptorSet DescriptorSetWrite{};
            DescriptorSetWrite.sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            DescriptorSetWrite.dstSet           = m_DescriptorSets[i];
            DescriptorSetWrite.dstBinding       = 0;
            DescriptorSetWrite.dstArrayElement  = 0;
            DescriptorSetWrite.descriptorType   = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            DescriptorSetWrite.descriptorCount  = 1;
            DescriptorSetWrite.pBufferInfo      = &DescriptorBufferInfo;
            DescriptorSetWrite.pImageInfo       = nullptr;
            DescriptorSetWrite.pTexelBufferView = nullptr;

            vkUpdateDescriptorSets(m_Device, 1, &DescriptorSetWrite, 0, nullptr);
        }
    }

} // namespace Corvus
