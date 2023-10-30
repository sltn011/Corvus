#include "CorvusPCH.h"

#include "Corvus/Renderer/Renderer.h"

namespace Corvus
{

    void CRenderer::CreatePerFrameDescriptorSets()
    {
        PerFrameDescriptorSets =
            AllocateDescriptorSets<s_FramesInFlight>(PerFrameDescriptorPool, PerFrameDescriptorSetLayout);

        // Allocated but not configured yet
        for (UInt32 i = 0; i < s_FramesInFlight; ++i)
        {
            std::array<VkWriteDescriptorSet, 1> DescriptorSetWrites{};

            // Write Uniform Buffer to UB Descriptor
            {
                VkDescriptorBufferInfo DescriptorBufferInfo{};
                DescriptorBufferInfo.buffer = MatricesUBOs[i].Buffer;
                DescriptorBufferInfo.offset = 0;
                DescriptorBufferInfo.range  = sizeof(CVPUBO);

                DescriptorSetWrites[0].sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                DescriptorSetWrites[0].dstSet           = PerFrameDescriptorSets[i];
                DescriptorSetWrites[0].dstBinding       = 0;
                DescriptorSetWrites[0].dstArrayElement  = 0;
                DescriptorSetWrites[0].descriptorType   = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                DescriptorSetWrites[0].descriptorCount  = 1;
                DescriptorSetWrites[0].pBufferInfo      = &DescriptorBufferInfo;
                DescriptorSetWrites[0].pImageInfo       = nullptr;
                DescriptorSetWrites[0].pTexelBufferView = nullptr;
            }

            vkUpdateDescriptorSets(
                Device, static_cast<UInt32>(DescriptorSetWrites.size()), DescriptorSetWrites.data(), 0, nullptr
            );
        }
    }

    void CRenderer::FreeDescriptorSets(VkDescriptorPool Pool, VkDescriptorSet *pSets, SizeT Amount)
    {
        vkFreeDescriptorSets(Device, Pool, static_cast<UInt32>(Amount), pSets);
        for (SizeT i = 0; i < Amount; ++i)
        {
            pSets[i] = VK_NULL_HANDLE;
        }
    }

} // namespace Corvus
