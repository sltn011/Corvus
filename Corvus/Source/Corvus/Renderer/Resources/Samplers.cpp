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
        Samplers.ClampNearest = Renderer().CreateSampler(
            VK_FILTER_NEAREST, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE, true, 16.f, VK_SAMPLER_MIPMAP_MODE_NEAREST, 1
        );

        Samplers.ClampLinear = Renderer().CreateSampler(
            VK_FILTER_LINEAR, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE, true, 16.f, VK_SAMPLER_MIPMAP_MODE_NEAREST, 1
        );

        Samplers.RepeatNearest = Renderer().CreateSampler(
            VK_FILTER_NEAREST, VK_SAMPLER_ADDRESS_MODE_REPEAT, true, 16.f, VK_SAMPLER_MIPMAP_MODE_NEAREST, 1
        );

        Samplers.RepeatLinear = Renderer().CreateSampler(
            VK_FILTER_LINEAR, VK_SAMPLER_ADDRESS_MODE_REPEAT, true, 16.f, VK_SAMPLER_MIPMAP_MODE_NEAREST, 1
        );

        Samplers.ClampNearest_Nearest = Renderer().CreateSampler(
            VK_FILTER_NEAREST, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE, true, 16.f, VK_SAMPLER_MIPMAP_MODE_NEAREST, 1000
        );

        Samplers.ClampLinear_Nearest = Renderer().CreateSampler(
            VK_FILTER_LINEAR, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE, true, 16.f, VK_SAMPLER_MIPMAP_MODE_NEAREST, 1000
        );
        Samplers.RepeatNearest_Nearest = Renderer().CreateSampler(
            VK_FILTER_NEAREST, VK_SAMPLER_ADDRESS_MODE_REPEAT, true, 16.f, VK_SAMPLER_MIPMAP_MODE_NEAREST, 1000
        );

        Samplers.RepeatLinear_Nearest = Renderer().CreateSampler(
            VK_FILTER_LINEAR, VK_SAMPLER_ADDRESS_MODE_REPEAT, true, 16.f, VK_SAMPLER_MIPMAP_MODE_NEAREST, 1000
        );

        Samplers.ClampNearest_Linear = Renderer().CreateSampler(
            VK_FILTER_NEAREST, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE, true, 16.f, VK_SAMPLER_MIPMAP_MODE_LINEAR, 1000
        );

        Samplers.ClampLinear_Linear = Renderer().CreateSampler(
            VK_FILTER_LINEAR, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE, true, 16.f, VK_SAMPLER_MIPMAP_MODE_LINEAR, 1000
        );

        Samplers.RepeatNearest_Linear = Renderer().CreateSampler(
            VK_FILTER_NEAREST, VK_SAMPLER_ADDRESS_MODE_REPEAT, true, 16.f, VK_SAMPLER_MIPMAP_MODE_LINEAR, 1000
        );

        Samplers.RepeatLinear_Linear = Renderer().CreateSampler(
            VK_FILTER_LINEAR, VK_SAMPLER_ADDRESS_MODE_REPEAT, true, 16.f, VK_SAMPLER_MIPMAP_MODE_LINEAR, 1000
        );
    }

    void CRenderer::DestroySamplers()
    {
        DestroySampler(Samplers.ClampNearest);
        DestroySampler(Samplers.ClampLinear);
        DestroySampler(Samplers.RepeatNearest);
        DestroySampler(Samplers.RepeatLinear);
        DestroySampler(Samplers.ClampNearest_Nearest);
        DestroySampler(Samplers.ClampLinear_Nearest);
        DestroySampler(Samplers.RepeatNearest_Nearest);
        DestroySampler(Samplers.RepeatLinear_Nearest);
        DestroySampler(Samplers.ClampNearest_Linear);
        DestroySampler(Samplers.ClampLinear_Linear);
        DestroySampler(Samplers.RepeatNearest_Linear);
        DestroySampler(Samplers.RepeatLinear_Linear);
    }

} // namespace Corvus
