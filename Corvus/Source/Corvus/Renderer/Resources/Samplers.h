#ifndef CORVUS_SOURCE_CORVUS_RENDERER_RESOURCES_SAMPLERS_H
#define CORVUS_SOURCE_CORVUS_RENDERER_RESOURCES_SAMPLERS_H

#include "Corvus/Core/Base.h"

#include <vulkan/vulkan.h>

namespace Corvus
{

    struct CSamplers
    {
        VkSampler FilteredSampler  = VK_NULL_HANDLE;
        VkSampler PixelatedSampler = VK_NULL_HANDLE;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_RENDERER_RESOURCES_SAMPLERS_H
