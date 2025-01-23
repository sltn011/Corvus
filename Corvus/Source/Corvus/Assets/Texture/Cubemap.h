#ifndef CORVUS_SOURCE_CORVUS_ASSETS_TEXTURE_CUBEMAP_H
#define CORVUS_SOURCE_CORVUS_ASSETS_TEXTURE_CUBEMAP_H

#include "Corvus/Assets/Texture/Texture.h"
#include "Corvus/Core/UUID.h"
#include "Corvus/Renderer/Resources/VulkanImage.h"

namespace Corvus
{

    class CCubemap
    {
    public:
        FUUID UUID;

        CVulkanImage EquirectangularImage;
        VkImageView  EquirectangularImageView;
        VkSampler    EquirectangularImageSampler;

        CVulkanImage CubemapImage;
        VkImageView  CubemapImageView;
        VkSampler    CubemapImageSampler;

        VkDescriptorSet DescriptorSet = VK_NULL_HANDLE; // 0: Equirectangular Image Sampler
                                                        // 1: Cubemap Image Sampler
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_ASSETS_TEXTURE_CUBEMAP_H
