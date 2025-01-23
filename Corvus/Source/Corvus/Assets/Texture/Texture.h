#ifndef CORVUS_SOURCE_CORVUS_ASSETS_TEXTURE_TEXTURE_H
#define CORVUS_SOURCE_CORVUS_ASSETS_TEXTURE_TEXTURE_H

#include "Corvus/Core/UUID.h"
#include "Corvus/Renderer/Resources/VulkanImage.h"

namespace Corvus
{

    class CTexture
    {
    public:
        FUUID        UUID;
        CVulkanImage Image;
        VkImageView  ImageView;
        VkSampler    Sampler;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_ASSETS_TEXTURE_TEXTURE_H
