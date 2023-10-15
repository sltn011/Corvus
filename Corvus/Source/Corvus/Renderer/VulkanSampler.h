#ifndef CORVUS_SOURCE_CORVUS_RENDERER_VULKANSAMPLER_H
#define CORVUS_SOURCE_CORVUS_RENDERER_VULKANSAMPLER_H

#include "Corvus/Core/Base.h"

#include <vulkan/vulkan.h>

namespace Corvus
{

    struct CVulkanSamplers
    {
        VkSampler DefaultSampler = VK_NULL_HANDLE;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_RENDERER_VULKANQUEUES_H
