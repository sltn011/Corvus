#ifndef CORVUS_SOURCE_CORVUS_ASSETS_MATERIAL_MATERIAL_H
#define CORVUS_SOURCE_CORVUS_ASSETS_MATERIAL_MATERIAL_H

#include "Corvus/Assets/Texture/Texture2D.h"

#include <vulkan/vulkan.h>

namespace Corvus
{

    struct CMaterial
    {
        CTexture2D      Albedo;
        VkDescriptorSet DescriptorSet;

        FUUID UUID;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_ASSETS_MATERIAL_MATERIAL_H
