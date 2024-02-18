#include "CorvusPCH.h"

#include "Corvus/Renderer/Renderer.h"

namespace Corvus
{

    void CRenderer::CreateDescriptorSetLayout()
    {
        // Per-Frame Descriptor Set Layout
        {
            std::vector<VkDescriptorSetLayoutBinding> Bindings(2);

            // Camera Uniform
            {
                Bindings[0] = VkInit::DescriptorSetLayoutBinding(
                    0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT
                );
            }

            // RenderTarget Uniform
            {
                Bindings[1] = VkInit::DescriptorSetLayoutBinding(
                    1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_FRAGMENT_BIT
                );
            }

            VkDescriptorSetLayoutCreateInfo DescriptorSetLayoutInfo =
                VkInit::DescriptorSetLayoutCreateInfo(Bindings.data(), Bindings.size());

            if (vkCreateDescriptorSetLayout(Device, &DescriptorSetLayoutInfo, nullptr, &PerFrameDescriptorSetLayout) !=
                VK_SUCCESS)
            {
                CORVUS_CORE_CRITICAL("Failed to create Vulkan Descriptor Set Layout!");
            }
            CORVUS_CORE_TRACE("Created Vulkan Descriptor Set Layout successfully");
        }

        // Per-Draw Descriptor Set Layout
        {
            std::vector<VkDescriptorSetLayoutBinding> Bindings(4);

            for (SizeT i = 0; i < Bindings.size(); ++i)
            {
                Bindings[i] = VkInit::DescriptorSetLayoutBinding(
                    static_cast<UInt32>(i), VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT
                );
            }

            VkDescriptorSetLayoutCreateInfo DescriptorSetLayoutInfo =
                VkInit::DescriptorSetLayoutCreateInfo(Bindings.data(), Bindings.size());

            if (vkCreateDescriptorSetLayout(Device, &DescriptorSetLayoutInfo, nullptr, &PerDrawDescriptorSetLayout) !=
                VK_SUCCESS)
            {
                CORVUS_CORE_CRITICAL("Failed to create Vulkan Descriptor Set Layout!");
            }
            CORVUS_CORE_TRACE("Created Vulkan Descriptor Set Layout successfully");
        }
    }

    void CRenderer::DestroyDescriptorSetLayout()
    {
        if (PerFrameDescriptorSetLayout)
        {
            vkDestroyDescriptorSetLayout(Device, PerFrameDescriptorSetLayout, nullptr);
            PerFrameDescriptorSetLayout = VK_NULL_HANDLE;
            CORVUS_CORE_TRACE("Vulkan Descriptor Set Layout destroyed");
        }
        if (PerDrawDescriptorSetLayout)
        {
            vkDestroyDescriptorSetLayout(Device, PerDrawDescriptorSetLayout, nullptr);
            PerDrawDescriptorSetLayout = VK_NULL_HANDLE;
            CORVUS_CORE_TRACE("Vulkan Descriptor Set Layout destroyed");
        }
    }

} // namespace Corvus
