#include "CorvusPCH.h"

#include "Corvus/Renderer/Renderer.h"

namespace Corvus
{

    void CRenderer::CreateDescriptorPools()
    {
        std::array<VkDescriptorPoolSize, 1> DescriptorPoolSizes{};
        DescriptorPoolSizes[0].type            = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        DescriptorPoolSizes[0].descriptorCount = static_cast<UInt32>(s_FramesInFlight);

        VkDescriptorPoolCreateInfo DescriptorPoolInfo{};
        DescriptorPoolInfo.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        DescriptorPoolInfo.poolSizeCount = static_cast<UInt32>(DescriptorPoolSizes.size());
        DescriptorPoolInfo.pPoolSizes    = DescriptorPoolSizes.data();
        DescriptorPoolInfo.maxSets       = static_cast<UInt32>(s_FramesInFlight);

        if (vkCreateDescriptorPool(m_Device, &DescriptorPoolInfo, nullptr, &m_DescriptorPool) != VK_SUCCESS)
        {
            CORVUS_CORE_CRITICAL("Failed to create Vulkan Descriptor Pool!");
        }

        VkDescriptorPoolSize GUIPoolSizes[] = {
            {VK_DESCRIPTOR_TYPE_SAMPLER, 32},
            {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 32},
            {VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 32},
            {VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 32},
            {VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 32},
            {VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 32},
            {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 32},
            {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 32},
            {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 32},
            {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 32},
            {VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 32}};

        VkDescriptorPoolCreateInfo GUIDescriptorPoolInfo = {};
        GUIDescriptorPoolInfo.sType                      = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        GUIDescriptorPoolInfo.flags                      = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        GUIDescriptorPoolInfo.maxSets                    = 32 * (sizeof(GUIPoolSizes) / sizeof(GUIPoolSizes[0]));
        GUIDescriptorPoolInfo.poolSizeCount              = (UInt32)(sizeof(GUIPoolSizes) / sizeof(GUIPoolSizes[0]));
        GUIDescriptorPoolInfo.pPoolSizes                 = GUIPoolSizes;

        if (vkCreateDescriptorPool(m_Device, &GUIDescriptorPoolInfo, nullptr, &m_GUIDescriptorPool) != VK_SUCCESS)
        {
            CORVUS_CORE_CRITICAL("Failed to create Vulkan Descriptor Pool!");
        }

        CORVUS_CORE_TRACE("Created Vulkan Descriptor Pool successfully");
    }

    void CRenderer::DestroyDescriptorPools()
    {
        if (m_DescriptorPool)
        {
            vkDestroyDescriptorPool(m_Device, m_DescriptorPool, nullptr);
            m_DescriptorPool = VK_NULL_HANDLE;
            CORVUS_CORE_TRACE("Vulkan Descriptor Pool destroyed");
        }
        if (m_GUIDescriptorPool)
        {
            vkDestroyDescriptorPool(m_Device, m_GUIDescriptorPool, nullptr);
            m_GUIDescriptorPool = VK_NULL_HANDLE;
            CORVUS_CORE_TRACE("Vulkan GUI Descriptor Pool destroyed");
        }
    }

} // namespace Corvus
