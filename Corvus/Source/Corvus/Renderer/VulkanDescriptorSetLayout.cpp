#include "CorvusPCH.h"

#include "Corvus/Renderer/Renderer.h"

namespace Corvus
{

    void CRenderer::CreateDescriptorSetLayout()
    {
        VkDescriptorSetLayoutBinding MatricesUBOLayoutBinding{};
        MatricesUBOLayoutBinding.binding            = 0;
        MatricesUBOLayoutBinding.descriptorType     = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        MatricesUBOLayoutBinding.descriptorCount    = 1; // > 1 to make uniform array
        MatricesUBOLayoutBinding.stageFlags         = VK_SHADER_STAGE_VERTEX_BIT;
        MatricesUBOLayoutBinding.pImmutableSamplers = nullptr; // not needed here

        VkDescriptorSetLayoutCreateInfo DescriptorSetLayoutInfo{};
        DescriptorSetLayoutInfo.sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        DescriptorSetLayoutInfo.bindingCount = 1;
        DescriptorSetLayoutInfo.pBindings    = &MatricesUBOLayoutBinding;

        if (vkCreateDescriptorSetLayout(m_Device, &DescriptorSetLayoutInfo, nullptr, &m_MatricesUBOLayout) !=
            VK_SUCCESS)
        {
            CORVUS_CRITICAL("Failed to create Vulkan Descriptor Set Layout!");
        }
        CORVUS_TRACE("Created Vulkan Descriptor Set Layout successfully");
    }

    void CRenderer::DestroyDescriptorSetLayout()
    {
        if (m_MatricesUBOLayout)
        {
            vkDestroyDescriptorSetLayout(m_Device, m_MatricesUBOLayout, nullptr);
            m_MatricesUBOLayout = VK_NULL_HANDLE;
            CORVUS_TRACE("Vulkan Descriptor Set Layout destroyed");
        }
    }

} // namespace Corvus
