#include "CorvusPCH.h"

#include "Corvus/Renderer/Renderer.h"

namespace Corvus
{

    void CRenderer::CreateDescriptorSetLayout()
    {
        std::array<VkDescriptorSetLayoutBinding, 1> Bindings;

        Bindings[0].binding            = 0;
        Bindings[0].descriptorType     = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        Bindings[0].descriptorCount    = 1; // > 1 to make uniform array
        Bindings[0].stageFlags         = VK_SHADER_STAGE_VERTEX_BIT;
        Bindings[0].pImmutableSamplers = nullptr; // not needed here
        Bindings[0].pImmutableSamplers = nullptr; // not needed here

        VkDescriptorSetLayoutCreateInfo DescriptorSetLayoutInfo{};
        DescriptorSetLayoutInfo.sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        DescriptorSetLayoutInfo.bindingCount = static_cast<UInt32>(Bindings.size());
        DescriptorSetLayoutInfo.pBindings    = Bindings.data();

        if (vkCreateDescriptorSetLayout(m_Device, &DescriptorSetLayoutInfo, nullptr, &m_DescriptorSetLayout) !=
            VK_SUCCESS)
        {
            CORVUS_CORE_CRITICAL("Failed to create Vulkan Descriptor Set Layout!");
        }
        CORVUS_CORE_TRACE("Created Vulkan Descriptor Set Layout successfully");
    }

    void CRenderer::DestroyDescriptorSetLayout()
    {
        if (m_DescriptorSetLayout)
        {
            vkDestroyDescriptorSetLayout(m_Device, m_DescriptorSetLayout, nullptr);
            m_DescriptorSetLayout = VK_NULL_HANDLE;
            CORVUS_CORE_TRACE("Vulkan Descriptor Set Layout destroyed");
        }
    }

} // namespace Corvus
