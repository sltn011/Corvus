#include "CorvusPCH.h"

#include "Corvus/Renderer/Renderer.h"

namespace Corvus
{

    void CRenderer::CreateDescriptorSetLayout()
    {
        // Per-Frame Descriptor Set Layout
        {
            std::array<VkDescriptorSetLayoutBinding, 1> Bindings;

            // ProjectionView Matrix
            {
                Bindings[0].binding            = 0;
                Bindings[0].descriptorType     = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                Bindings[0].descriptorCount    = 1; // > 1 to make uniform array
                Bindings[0].stageFlags         = VK_SHADER_STAGE_VERTEX_BIT;
                Bindings[0].pImmutableSamplers = nullptr; // not needed here
                Bindings[0].pImmutableSamplers = nullptr; // not needed here
            }

            VkDescriptorSetLayoutCreateInfo DescriptorSetLayoutInfo{};
            DescriptorSetLayoutInfo.sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            DescriptorSetLayoutInfo.bindingCount = static_cast<UInt32>(Bindings.size());
            DescriptorSetLayoutInfo.pBindings    = Bindings.data();

            if (vkCreateDescriptorSetLayout(Device, &DescriptorSetLayoutInfo, nullptr, &PerFrameDescriptorSetLayout) !=
                VK_SUCCESS)
            {
                CORVUS_CORE_CRITICAL("Failed to create Vulkan Descriptor Set Layout!");
            }
            CORVUS_CORE_TRACE("Created Vulkan Descriptor Set Layout successfully");
        }

        // Per-Draw Descriptor Set Layout
        {
            std::array<VkDescriptorSetLayoutBinding, 4> Bindings;

            for (SizeT i = 0; i < Bindings.size(); ++i)
            {
                Bindings[i].binding            = static_cast<UInt32>(i);
                Bindings[i].descriptorType     = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                Bindings[i].descriptorCount    = 1; // > 1 to make uniform array
                Bindings[i].stageFlags         = VK_SHADER_STAGE_FRAGMENT_BIT;
                Bindings[i].pImmutableSamplers = nullptr; // not needed here
                Bindings[i].pImmutableSamplers = nullptr; // not needed here
            }

            VkDescriptorSetLayoutCreateInfo DescriptorSetLayoutInfo{};
            DescriptorSetLayoutInfo.sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            DescriptorSetLayoutInfo.bindingCount = static_cast<UInt32>(Bindings.size());
            DescriptorSetLayoutInfo.pBindings    = Bindings.data();

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