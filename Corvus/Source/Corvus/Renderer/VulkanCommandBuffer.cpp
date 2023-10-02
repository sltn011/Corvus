#include "CorvusPCH.h"

#include "Corvus/Renderer/Renderer.h"

namespace Corvus
{

    void CRenderer::AllocateCommandBuffers()
    {
        VkCommandBufferAllocateInfo CommandBufferInfo{};
        CommandBufferInfo.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        CommandBufferInfo.commandPool        = m_CommandPool;
        CommandBufferInfo.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        CommandBufferInfo.commandBufferCount = static_cast<UInt32>(m_CommandBuffers.size());

        if (vkAllocateCommandBuffers(m_Device, &CommandBufferInfo, m_CommandBuffers.data()) != VK_SUCCESS)
        {
            CORVUS_CORE_CRITICAL("Failed to allocate Vulkan Command Buffer!");
        }
        CORVUS_CORE_TRACE("Allocated Vulkan Command Buffer successfully");
    }

} // namespace Corvus
