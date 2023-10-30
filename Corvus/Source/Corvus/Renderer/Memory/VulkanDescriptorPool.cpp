#include "CorvusPCH.h"

#include "Corvus/Renderer/Renderer.h"

namespace Corvus
{

    void CRenderer::CreateDescriptorPools()
    {
        // Per-Frame Pool
        {
            VkDescriptorPoolSize DescriptorPoolSizes[] = {
                {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, static_cast<UInt32>(s_FramesInFlight)},
                {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 4 * static_cast<UInt32>(s_FramesInFlight)}};

            VkDescriptorPoolCreateInfo DescriptorPoolInfo{};
            DescriptorPoolInfo.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
            DescriptorPoolInfo.poolSizeCount = (UInt32)(sizeof(DescriptorPoolSizes) / sizeof(DescriptorPoolSizes[0]));
            DescriptorPoolInfo.pPoolSizes    = DescriptorPoolSizes;
            DescriptorPoolInfo.maxSets       = static_cast<UInt32>(s_FramesInFlight);

            if (vkCreateDescriptorPool(Device, &DescriptorPoolInfo, nullptr, &PerFrameDescriptorPool) != VK_SUCCESS)
            {
                CORVUS_CORE_CRITICAL("Failed to create Vulkan Descriptor Pool!");
            }
            CORVUS_CORE_TRACE("Created Vulkan Descriptor Pool successfully");
        }

        // Per-Draw pool
        {
            VkDescriptorPoolSize DescriptorPoolSizes[] = {{VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 4 * 64}};

            VkDescriptorPoolCreateInfo DescriptorPoolInfo{};
            DescriptorPoolInfo.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
            DescriptorPoolInfo.poolSizeCount = (UInt32)(sizeof(DescriptorPoolSizes) / sizeof(DescriptorPoolSizes[0]));
            DescriptorPoolInfo.pPoolSizes    = DescriptorPoolSizes;
            DescriptorPoolInfo.maxSets       = 64;
            DescriptorPoolInfo.flags         = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;

            if (vkCreateDescriptorPool(Device, &DescriptorPoolInfo, nullptr, &PerDrawDescriptorPool) != VK_SUCCESS)
            {
                CORVUS_CORE_CRITICAL("Failed to create Vulkan Descriptor Pool!");
            }
            CORVUS_CORE_TRACE("Created Vulkan Descriptor Pool successfully");
        }

        // GUI Pool
        {
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

            GUIDescriptorPoolInfo.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
            GUIDescriptorPoolInfo.flags         = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
            GUIDescriptorPoolInfo.maxSets       = (UInt32)(32 * (sizeof(GUIPoolSizes) / sizeof(GUIPoolSizes[0])));
            GUIDescriptorPoolInfo.poolSizeCount = (UInt32)(sizeof(GUIPoolSizes) / sizeof(GUIPoolSizes[0]));
            GUIDescriptorPoolInfo.pPoolSizes    = GUIPoolSizes;

            if (vkCreateDescriptorPool(Device, &GUIDescriptorPoolInfo, nullptr, &GUIDescriptorPool) != VK_SUCCESS)
            {
                CORVUS_CORE_CRITICAL("Failed to create Vulkan Descriptor Pool!");
            }
            CORVUS_CORE_TRACE("Created Vulkan Descriptor Pool successfully");
        }
    }

    void CRenderer::DestroyDescriptorPools()
    {
        if (PerFrameDescriptorPool)
        {
            vkDestroyDescriptorPool(Device, PerFrameDescriptorPool, nullptr);
            PerFrameDescriptorPool = VK_NULL_HANDLE;
            CORVUS_CORE_TRACE("Vulkan Descriptor Pool destroyed");
        }
        if (PerDrawDescriptorPool)
        {
            vkDestroyDescriptorPool(Device, PerDrawDescriptorPool, nullptr);
            PerDrawDescriptorPool = VK_NULL_HANDLE;
            CORVUS_CORE_TRACE("Vulkan Descriptor Pool destroyed");
        }
        if (GUIDescriptorPool)
        {
            vkDestroyDescriptorPool(Device, GUIDescriptorPool, nullptr);
            GUIDescriptorPool = VK_NULL_HANDLE;
            CORVUS_CORE_TRACE("Vulkan GUI Descriptor Pool destroyed");
        }
    }

} // namespace Corvus
