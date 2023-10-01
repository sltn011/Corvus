#ifndef CORVUS_SOURCE_CORVUS_RENDERER_VULKANQUEUES_H
#define CORVUS_SOURCE_CORVUS_RENDERER_VULKANQUEUES_H

#include "Corvus/Core/Base.h"

#include <vulkan/vulkan.h>

namespace Corvus
{

    struct CVulkanQueues
    {
        VkQueue GraphicsQueue     = VK_NULL_HANDLE;
        VkQueue PresentationQueue = VK_NULL_HANDLE;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_RENDERER_VULKANQUEUES_H
