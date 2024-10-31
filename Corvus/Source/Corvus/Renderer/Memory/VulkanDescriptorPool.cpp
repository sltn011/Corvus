#include "CorvusPCH.h"

#include "Corvus/Renderer/Renderer.h"

namespace Corvus
{

    void CRenderer::CreateDescriptorPools()
    {
        // Per-Frame Pool
        {
            std::vector<VkDescriptorPoolSize> DescriptorPoolSizes = {
                VkInit::DescriptorPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 2)};

            VkDescriptorPoolCreateInfo DescriptorPoolInfo = VkInit::DescriptorPoolCreateInfo(
                DescriptorPoolSizes.data(), DescriptorPoolSizes.size(), s_FramesInFlight, 0
            );

            if (vkCreateDescriptorPool(Device, &DescriptorPoolInfo, nullptr, &PerFrameDescriptorPool) != VK_SUCCESS)
            {
                CORVUS_CORE_CRITICAL("Failed to create Vulkan Descriptor Pool!");
            }
            CORVUS_CORE_TRACE("Created Vulkan Descriptor Pool successfully");
        }

        // Per-Draw pool
        {
            std::vector<VkDescriptorPoolSize> DescriptorPoolSizes = {
                VkInit::DescriptorPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 4)};

            VkDescriptorPoolCreateInfo DescriptorPoolInfo = VkInit::DescriptorPoolCreateInfo(
                DescriptorPoolSizes.data(),
                DescriptorPoolSizes.size(),
                64,
                VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT
            );

            if (vkCreateDescriptorPool(Device, &DescriptorPoolInfo, nullptr, &PerDrawDescriptorPool) != VK_SUCCESS)
            {
                CORVUS_CORE_CRITICAL("Failed to create Vulkan Descriptor Pool!");
            }
            CORVUS_CORE_TRACE("Created Vulkan Descriptor Pool successfully");
        }

        // GUI Pool
        {
            std::vector<VkDescriptorPoolSize> GUIPoolSizes = {
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

            VkDescriptorPoolCreateInfo GUIDescriptorPoolInfo = VkInit::DescriptorPoolCreateInfo(
                GUIPoolSizes.data(), GUIPoolSizes.size(), 64, VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT
            );

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
