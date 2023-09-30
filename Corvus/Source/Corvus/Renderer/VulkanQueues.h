#ifndef CORVUS_SOURCE_CORVUS_RENDERER_VULKANQUEUES_H
#define CORVUS_SOURCE_CORVUS_RENDERER_VULKANQUEUES_H

#include "Corvus/Core/Base.h"

#include <vulkan/vulkan.h>

namespace Corvus
{

    class CVulkanQueues
    {
    public:
        VkQueue m_GraphicsQueueHandler     = VK_NULL_HANDLE;
        VkQueue m_PresentationQueueHandler = VK_NULL_HANDLE;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_RENDERER_VULKANQUEUES_H
