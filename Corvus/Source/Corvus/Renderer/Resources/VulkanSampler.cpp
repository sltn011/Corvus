#include "CorvusPCH.h"

#include "Corvus/Renderer/Renderer.h"

namespace Corvus
{

    void CRenderer::CreateSamplers()
    {
        m_Samplers.DefaultSampler = CreateSampler(
            VK_FILTER_LINEAR, VK_SAMPLER_ADDRESS_MODE_REPEAT, true, 16.f, VK_SAMPLER_MIPMAP_MODE_LINEAR, 1000
        );
    }

    void CRenderer::DestroySamplers()
    {
        if (m_Samplers.DefaultSampler != VK_NULL_HANDLE)
        {
            vkDestroySampler(m_Device, m_Samplers.DefaultSampler, nullptr);
            m_Samplers.DefaultSampler = VK_NULL_HANDLE;
        }
    }

    VkSampler CRenderer::CreateSampler(
        VkFilter             MinMagFilter,
        VkSamplerAddressMode AddressMode,
        bool                 bAnisoEnabled,
        float                Anisotropy,
        VkSamplerMipmapMode  Filtering,
        UInt32               MipLevels
    ) const
    {
        VkSampler Sampler = VK_NULL_HANDLE;

        float MaxAnisotropy = m_PhysicalDeviceProperties.limits.maxSamplerAnisotropy;

        VkSamplerCreateInfo SamplerCreateInfo{};
        SamplerCreateInfo.sType                   = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        SamplerCreateInfo.minFilter               = MinMagFilter;
        SamplerCreateInfo.magFilter               = MinMagFilter;
        SamplerCreateInfo.addressModeU            = AddressMode;
        SamplerCreateInfo.addressModeV            = AddressMode;
        SamplerCreateInfo.addressModeW            = AddressMode;
        SamplerCreateInfo.anisotropyEnable        = bAnisoEnabled ? VK_TRUE : VK_FALSE;
        SamplerCreateInfo.maxAnisotropy           = FMath::Min(MaxAnisotropy, Anisotropy);
        SamplerCreateInfo.borderColor             = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        SamplerCreateInfo.unnormalizedCoordinates = VK_FALSE;
        SamplerCreateInfo.compareEnable           = VK_FALSE;
        SamplerCreateInfo.compareOp               = VK_COMPARE_OP_ALWAYS;
        SamplerCreateInfo.mipmapMode              = Filtering;
        SamplerCreateInfo.mipLodBias              = 0.0f;
        SamplerCreateInfo.minLod                  = 0.0f;
        SamplerCreateInfo.maxLod                  = static_cast<float>(MipLevels);

        if (vkCreateSampler(m_Device, &SamplerCreateInfo, nullptr, &Sampler) != VK_SUCCESS)
        {
            CORVUS_CORE_CRITICAL("Failed to create Vulkan Sampler!");
        }

        return Sampler;
    }

    CVulkanSamplers CRenderer::GetSamplers() const
    {
        return m_Samplers;
    }

} // namespace Corvus
