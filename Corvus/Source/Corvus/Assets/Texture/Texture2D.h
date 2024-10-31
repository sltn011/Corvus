#ifndef CORVUS_SOURCE_CORVUS_ASSETS_TEXTURE_TEXTURE2D_H
#define CORVUS_SOURCE_CORVUS_ASSETS_TEXTURE_TEXTURE2D_H

#include "Corvus/Core/UUID.h"
#include "Corvus/Renderer/Resources/VulkanImage.h"

namespace Corvus
{

    class CTexture2D
    {
    public:
        FUUID        UUID;
        CVulkanImage Image;
        VkImageView  ImageView;
        VkSampler    Sampler;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_ASSETS_IMAGE_IMAGEDATA_H
