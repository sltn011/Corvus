#ifndef CORVUS_SOURCE_CORVUS_ASSETS_MATERIAL_MATERIAL_H
#define CORVUS_SOURCE_CORVUS_ASSETS_MATERIAL_MATERIAL_H

#include "Corvus/Assets/Texture/Texture.h"

#include <vulkan/vulkan.h>

namespace Corvus
{

    class CMaterial
    {
    public:
        CTexture      Albedo;
        CTexture      Normal;
        VkDescriptorSet DescriptorSet = VK_NULL_HANDLE;

        FUUID UUID;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_ASSETS_MATERIAL_MATERIAL_H
