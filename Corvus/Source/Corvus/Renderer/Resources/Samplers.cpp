#include "CorvusPCH.h"

#include "Corvus/Renderer/Renderer.h"

namespace Corvus
{

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

        VkSamplerCreateInfo SamplerCreateInfo = VkInit::SamplerCreateInfo(
            MinMagFilter, AddressMode, bAnisoEnabled, FMath::Min(MaxAnisotropy, Anisotropy), Filtering, MipLevels
        );

        if (vkCreateSampler(Device, &SamplerCreateInfo, nullptr, &Sampler) != VK_SUCCESS)
        {
            CORVUS_CORE_CRITICAL("Failed to create Vulkan Sampler!");
        }

        return Sampler;
    }

    void CRenderer::DestroySampler(VkSampler &Sampler)
    {
        if (Sampler != VK_NULL_HANDLE)
        {
            vkDestroySampler(Device, Sampler, nullptr);
            Sampler = VK_NULL_HANDLE;
        }
    }

    void CRenderer::CreateSamplers()
    {
        Samplers.FilteredSampler = Renderer().CreateSampler(
            VK_FILTER_LINEAR, VK_SAMPLER_ADDRESS_MODE_REPEAT, true, 16.f, VK_SAMPLER_MIPMAP_MODE_LINEAR, 1000
        );

        Samplers.PixelatedSampler = Renderer().CreateSampler(
            VK_FILTER_NEAREST, VK_SAMPLER_ADDRESS_MODE_REPEAT, true, 16.f, VK_SAMPLER_MIPMAP_MODE_LINEAR, 1000
        );
    }

    void CRenderer::DestroySamplers()
    {
        DestroySampler(Samplers.FilteredSampler);
        DestroySampler(Samplers.PixelatedSampler);
    }

} // namespace Corvus
