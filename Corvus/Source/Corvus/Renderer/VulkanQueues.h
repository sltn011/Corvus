#ifndef CORVUS_SOURCE_CORVUS_RENDERER_VULKANQUEUES_H
#define CORVUS_SOURCE_CORVUS_RENDERER_VULKANQUEUES_H

#include "Corvus/Core/Base.h"
#include "Corvus/Renderer/VulkanDevice.h"
#include "Corvus/Renderer/VulkanQueueFamilyIndices.h"

#include <vulkan/vulkan.h>

namespace Corvus
{

    class CVulkanQueues
    {
    public:
        friend class CVulkanDevice;

        VkQueue GraphicsQueueHandler() const;
        VkQueue PresentationQueueHandler() const;

        CVulkanQueueFamilyIndices const &QueueFamilyIndices() const;

    private:
        VkQueue m_GraphicsQueueHandler     = VK_NULL_HANDLE;
        VkQueue m_PresentationQueueHandler = VK_NULL_HANDLE;

        CVulkanQueueFamilyIndices m_QueueFamilyIndices;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_RENDERER_VULKANQUEUES_H
