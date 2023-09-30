#ifndef CORVUS_SOURCE_CORVUS_RENDERER_VULKANIMAGEVIEWPARAMS_H
#define CORVUS_SOURCE_CORVUS_RENDERER_VULKANIMAGEVIEWPARAMS_H

#include "Corvus/Core/Base.h"

#include <vulkan/vulkan.h>

struct CVulkanImageViewParams
{
    VkFormat              Format;
    VkImageAspectFlagBits AspectMask;
    uint32_t              NumMips;
};

#endif // !CORVUS_SOURCE_CORVUS_RENDERER_VULKANIMAGEVIEWPARAMS_H
