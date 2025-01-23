#ifndef CORVUS_SOURCE_CORVUS_ASSETS_DEFAULT_DEFAULTTEXTURES_H
#define CORVUS_SOURCE_CORVUS_ASSETS_DEFAULT_DEFAULTTEXTURES_H

#include "Corvus/Assets/Texture/Texture.h"

#include <vulkan/vulkan.h>

namespace Corvus
{

    struct CDefaultTextures
    {
        void Create();

        CTexture Albedo;
        CTexture Normal;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_ASSETS_DEFAULT_DEFAULTTEXTURES_H
