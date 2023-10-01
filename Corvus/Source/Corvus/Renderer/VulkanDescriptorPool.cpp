#include "CorvusPCH.h"

#include "Corvus/Renderer/Renderer.h"

namespace Corvus
{

    void CRenderer::CreateDescriptorPool()
    {
        VkDescriptorPoolSize DescriptorPoolSize{};
        DescriptorPoolSize.type            = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        DescriptorPoolSize.descriptorCount = static_cast<uint32_t>(s_FramesInFlight);

        VkDescriptorPoolCreateInfo DescriptorPoolInfo{};
        DescriptorPoolInfo.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        DescriptorPoolInfo.poolSizeCount = 1;
        DescriptorPoolInfo.pPoolSizes    = &DescriptorPoolSize;
        DescriptorPoolInfo.maxSets       = static_cast<uint32_t>(s_FramesInFlight);

        if (vkCreateDescriptorPool(m_Device, &DescriptorPoolInfo, nullptr, &m_DescriptorPool) != VK_SUCCESS)
        {
            CORVUS_CRITICAL("Failed to create Vulkan Descriptor Pool!");
        }
        CORVUS_TRACE("Created Vulkan Descriptor Pool successfully");
    }

    void CRenderer::DestroyDescriptorPool()
    {
        if (m_DescriptorPool)
        {
            vkDestroyDescriptorPool(m_Device, m_DescriptorPool, nullptr);
            m_DescriptorPool = VK_NULL_HANDLE;
            CORVUS_TRACE("Vulkan Descriptor Pool destroyed");
        }
    }

} // namespace Corvus
