#ifndef CORVUS_SOURCE_CORVUS_RENDERER_VULKANIMAGE_H
#define CORVUS_SOURCE_CORVUS_RENDERER_VULKANIMAGE_H

#include "Corvus/Core/Base.h"

#include <vulkan/vulkan.h>

namespace Corvus
{

    struct CVulkanImage
    {
        VkImage        Image     = VK_NULL_HANDLE;
        VkDeviceMemory Memory    = VK_NULL_HANDLE;
        UInt32         MipLevels = 1;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_RENDERER_VULKANIMAGE_H
