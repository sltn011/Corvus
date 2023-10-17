#include "CorvusPCH.h"

#include "Corvus/Renderer/Core/VulkanQueues.h"

#include "Corvus/Renderer/Renderer.h"

namespace Corvus
{
    void CRenderer::RetrieveQueues()
    {
        UInt32 QueueIndex = 0;
        vkGetDeviceQueue(m_Device, m_QueueFamilyIndices.GraphicsFamily.value(), QueueIndex, &m_Queues.GraphicsQueue);
        vkGetDeviceQueue(
            m_Device, m_QueueFamilyIndices.PresentationFamily.value(), QueueIndex, &m_Queues.PresentationQueue
        );

        CORVUS_CORE_TRACE("Retrieved Queues from Vulkan Device");
    }
} // namespace Corvus
