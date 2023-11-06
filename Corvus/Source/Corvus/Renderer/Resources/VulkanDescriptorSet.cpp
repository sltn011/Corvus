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
            std::vector<VkWriteDescriptorSet>   DescriptorSetWrites(1);
            std::vector<VkDescriptorBufferInfo> DescriptorBufferInfo(1);

            // Write Uniform Buffer to UB Descriptor
            {
                DescriptorBufferInfo[0] = {VkInit::DescriptorBufferInfo(MatricesUBOs[i].Buffer, 0, sizeof(CVPUBO))};

                DescriptorSetWrites[0] = VkInit::WriteDescriptorSet(
                    PerFrameDescriptorSets[i],
                    0,
                    0,
                    VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                    DescriptorBufferInfo.data(),
                    nullptr,
                    nullptr,
                    DescriptorBufferInfo.size()
                );
            }

            vkUpdateDescriptorSets(
                Device, static_cast<UInt32>(DescriptorSetWrites.size()), DescriptorSetWrites.data(), 0, nullptr
            );
        }
    } // namespace Corvus

    void CRenderer::FreeDescriptorSets(VkDescriptorPool Pool, VkDescriptorSet *pSets, SizeT Amount)
    {
        vkFreeDescriptorSets(Device, Pool, static_cast<UInt32>(Amount), pSets);
        for (SizeT i = 0; i < Amount; ++i)
        {
            pSets[i] = VK_NULL_HANDLE;
        }
    }

} // namespace Corvus
