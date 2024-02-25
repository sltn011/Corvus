#ifndef CORVUS_SOURCE_CORVUS_RENDERER_RESOURCES_SAMPLERS_H
#define CORVUS_SOURCE_CORVUS_RENDERER_RESOURCES_SAMPLERS_H

#include "Corvus/Core/Base.h"

#include <vulkan/vulkan.h>

namespace Corvus
{

    struct CSamplers
    {
        VkSampler ClampNearest          = VK_NULL_HANDLE; // clamp UV, nearest interpolation, no mipmap
        VkSampler ClampLinear           = VK_NULL_HANDLE; // clamp UV, linear interpolation, no mipmap
        VkSampler RepeatNearest         = VK_NULL_HANDLE; // repeat UV, nearest interpolation, no mipmap
        VkSampler RepeatLinear          = VK_NULL_HANDLE; // repeat UV, linear interpolation, no mipmap
        VkSampler ClampNearest_Nearest  = VK_NULL_HANDLE; // clamp UV, nearest interpolation, nearest mipmap
        VkSampler ClampLinear_Nearest   = VK_NULL_HANDLE; // clamp UV, linear interpolation, nearest mipmap
        VkSampler RepeatNearest_Nearest = VK_NULL_HANDLE; // repeat UV, nearest interpolation, nearest mipmap
        VkSampler RepeatLinear_Nearest  = VK_NULL_HANDLE; // repeat UV, linear interpolation, nearest mipmap
        VkSampler ClampNearest_Linear   = VK_NULL_HANDLE; // clamp UV, nearest interpolation, linear mipmap
        VkSampler ClampLinear_Linear    = VK_NULL_HANDLE; // clamp UV, linear interpolation, linear mipmap
        VkSampler RepeatNearest_Linear  = VK_NULL_HANDLE; // repeat UV, nearest interpolation, linear mipmap
        VkSampler RepeatLinear_Linear   = VK_NULL_HANDLE; // repeat UV, linear interpolation, linear mipmap

        static constexpr UInt32 NumSamplers = 12;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_RENDERER_RESOURCES_SAMPLERS_H
