#include "CorvusPCH.h"

#include "Corvus/Renderer/Renderer.h"

namespace Corvus
{

    void CRenderer::AllocateDescriptorSets()
    {
        std::array<VkDescriptorSetLayout, s_FramesInFlight> SetsLayouts{};
        SetsLayouts.fill(m_DescriptorSetLayout);

        VkDescriptorSetAllocateInfo DescriptorSetInfo{};
        DescriptorSetInfo.sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        DescriptorSetInfo.descriptorPool     = m_DescriptorPool;
        DescriptorSetInfo.descriptorSetCount = static_cast<UInt32>(s_FramesInFlight);
        DescriptorSetInfo.pSetLayouts        = SetsLayouts.data();

        if (vkAllocateDescriptorSets(m_Device, &DescriptorSetInfo, m_DescriptorSets.data()) != VK_SUCCESS)
        {
            CORVUS_CORE_CRITICAL("Failed to allocate Vulkan Descriptor Sets!");
        }
        CORVUS_CORE_TRACE("Allocated Vulkan Descriptor Sets successfully");

        // Allocated but not configured yet
        for (UInt32 i = 0; i < s_FramesInFlight; ++i)
        {
            std::array<VkWriteDescriptorSet, 1> DescriptorSetWrites{};

            // Write Uniform Buffer to UB Descriptor
            {
                VkDescriptorBufferInfo DescriptorBufferInfo{};
                DescriptorBufferInfo.buffer = m_MatricesUBOs[i].Buffer;
                DescriptorBufferInfo.offset = 0;
                DescriptorBufferInfo.range  = sizeof(CVPUBO);

                DescriptorSetWrites[0].sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                DescriptorSetWrites[0].dstSet           = m_DescriptorSets[i];
                DescriptorSetWrites[0].dstBinding       = 0;
                DescriptorSetWrites[0].dstArrayElement  = 0;
                DescriptorSetWrites[0].descriptorType   = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                DescriptorSetWrites[0].descriptorCount  = 1;
                DescriptorSetWrites[0].pBufferInfo      = &DescriptorBufferInfo;
                DescriptorSetWrites[0].pImageInfo       = nullptr;
                DescriptorSetWrites[0].pTexelBufferView = nullptr;
            }

            vkUpdateDescriptorSets(
                m_Device, static_cast<UInt32>(DescriptorSetWrites.size()), DescriptorSetWrites.data(), 0, nullptr
            );
        }
    }

} // namespace Corvus
