#include "CorvusPCH.h"

#include "Corvus/Renderer/VulkanQueues.h"

namespace Corvus
{

    VkQueue CVulkanQueues::GraphicsQueueHandler() const
    {
        return m_GraphicsQueueHandler;
    }

    VkQueue CVulkanQueues::PresentationQueueHandler() const
    {
        return m_PresentationQueueHandler;
    }

    CVulkanQueueFamilyIndices const &CVulkanQueues::QueueFamilyIndices() const
    {
        return m_QueueFamilyIndices;
    }

} // namespace Corvus
