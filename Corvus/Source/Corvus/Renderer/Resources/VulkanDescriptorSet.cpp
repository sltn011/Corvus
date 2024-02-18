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
            std::vector<VkWriteDescriptorSet>   DescriptorSetWrites(2);
            std::vector<VkDescriptorBufferInfo> DescriptorBufferInfo(2);

            // Write Uniform Buffer to UB Descriptor
            {
                DescriptorBufferInfo[0] = {VkInit::DescriptorBufferInfo(CameraUBOs[i].Buffer, 0, sizeof(CCameraUBO))};
                DescriptorSetWrites[0]  = VkInit::WriteDescriptorSet(
                    PerFrameDescriptorSets[i],
                    0,
                    0,
                    VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                    &DescriptorBufferInfo[0],
                    nullptr,
                    nullptr,
                    1
                );

                DescriptorBufferInfo[1] = {
                    VkInit::DescriptorBufferInfo(RenderTargetUBOs[i].Buffer, 0, sizeof(CRenderTargetUBO))};
                DescriptorSetWrites[1] = VkInit::WriteDescriptorSet(
                    PerFrameDescriptorSets[i],
                    1,
                    0,
                    VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                    &DescriptorBufferInfo[1],
                    nullptr,
                    nullptr,
                    1
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
