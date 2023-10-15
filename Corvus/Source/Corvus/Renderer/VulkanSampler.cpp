#include "CorvusPCH.h"

#include "Corvus/Renderer/Renderer.h"

namespace Corvus
{

    void CRenderer::CreateSamplers()
    {
        VkPhysicalDeviceProperties DeviceProperties = GetPhysicalDeviceProperties(m_PhysicalDevice);
        float                      MaxAnisotropy    = DeviceProperties.limits.maxSamplerAnisotropy;

        // Default Sampler
        {
            VkSamplerCreateInfo SamplerCreateInfo{};
            SamplerCreateInfo.sType                   = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
            SamplerCreateInfo.minFilter               = VK_FILTER_LINEAR;
            SamplerCreateInfo.magFilter               = VK_FILTER_LINEAR;
            SamplerCreateInfo.addressModeU            = VK_SAMPLER_ADDRESS_MODE_REPEAT;
            SamplerCreateInfo.addressModeV            = VK_SAMPLER_ADDRESS_MODE_REPEAT;
            SamplerCreateInfo.addressModeW            = VK_SAMPLER_ADDRESS_MODE_REPEAT;
            SamplerCreateInfo.anisotropyEnable        = VK_TRUE;
            SamplerCreateInfo.maxAnisotropy           = FMath::Min(MaxAnisotropy, 16.f);
            SamplerCreateInfo.borderColor             = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
            SamplerCreateInfo.unnormalizedCoordinates = VK_FALSE;
            SamplerCreateInfo.compareEnable           = VK_FALSE;
            SamplerCreateInfo.compareOp               = VK_COMPARE_OP_ALWAYS;
            SamplerCreateInfo.mipmapMode              = VK_SAMPLER_MIPMAP_MODE_LINEAR;
            SamplerCreateInfo.mipLodBias              = 0.0f;
            SamplerCreateInfo.minLod                  = 0.0f;
            SamplerCreateInfo.maxLod                  = 0.0f;

            if (vkCreateSampler(m_Device, &SamplerCreateInfo, nullptr, &m_Samplers.DefaultSampler) != VK_SUCCESS)
            {
                CORVUS_CORE_CRITICAL("Failed to create Vulkan Samplers!");
            }
        }
    }

    void CRenderer::DestroySamplers()
    {
        if (m_Samplers.DefaultSampler != VK_NULL_HANDLE)
        {
            vkDestroySampler(m_Device, m_Samplers.DefaultSampler, nullptr);
            m_Samplers.DefaultSampler = VK_NULL_HANDLE;
        }
    }

    CVulkanSamplers CRenderer::GetSamplers() const
    {
        return m_Samplers;
    }

} // namespace Corvus
