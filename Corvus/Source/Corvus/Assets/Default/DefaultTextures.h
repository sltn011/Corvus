#ifndef CORVUS_SOURCE_CORVUS_ASSETS_DEFAULT_DEFAULTTEXTURES_H
#define CORVUS_SOURCE_CORVUS_ASSETS_DEFAULT_DEFAULTTEXTURES_H

#include "Corvus/Assets/Texture/Texture2D.h"

#include <vulkan/vulkan.h>

namespace Corvus
{

    struct CDefaultTextures
    {
        void Create();

        CTexture2D Albedo;
        CTexture2D Normal;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_ASSETS_DEFAULT_DEFAULTTEXTURES_H
