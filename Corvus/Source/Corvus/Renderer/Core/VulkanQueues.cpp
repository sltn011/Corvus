#include "CorvusPCH.h"

#include "Corvus/Renderer/Core/VulkanQueues.h"

#include "Corvus/Renderer/Renderer.h"

namespace Corvus
{
    void CRenderer::RetrieveQueues()
    {
        UInt32 QueueIndex = 0;
        vkGetDeviceQueue(Device, QueueFamilyIndices.GraphicsFamily.value(), QueueIndex, &Queues.GraphicsQueue);
        vkGetDeviceQueue(
            Device, QueueFamilyIndices.PresentationFamily.value(), QueueIndex, &Queues.PresentationQueue
        );

        CORVUS_CORE_TRACE("Retrieved Queues from Vulkan Device");
    }
} // namespace Corvus
