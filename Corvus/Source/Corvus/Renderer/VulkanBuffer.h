#ifndef CORVUS_SOURCE_CORVUS_RENDERER_VULKANBUFFER_H
#define CORVUS_SOURCE_CORVUS_RENDERER_VULKANBUFFER_H

#include "Corvus/Core/Base.h"

#include <vulkan/vulkan.h>

namespace Corvus
{

    struct CVulkanBuffer
    {
        VkBuffer       Buffer = VK_NULL_HANDLE;
        VkDeviceMemory Memory = VK_NULL_HANDLE;
    };

    struct CVulkanUniformBuffer
    {
        VkBuffer       Buffer       = VK_NULL_HANDLE;
        VkDeviceMemory Memory       = VK_NULL_HANDLE;
        void          *MappedMemory = nullptr;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_RENDERER_VULKANBUFFER_H
