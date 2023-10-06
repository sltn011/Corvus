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

    VkCommandBuffer CRenderer::BeginSingleTimeCommand()
    {
        VkCommandBufferAllocateInfo BufferAllocateInfo{};
        BufferAllocateInfo.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        BufferAllocateInfo.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        BufferAllocateInfo.commandPool        = m_CommandPool;
        BufferAllocateInfo.commandBufferCount = 1;

        VkCommandBuffer CommandBuffer;
        if (vkAllocateCommandBuffers(m_Device, &BufferAllocateInfo, &CommandBuffer) != VK_SUCCESS)
        {
            CORVUS_CORE_CRITICAL("Failed to allocate Vulkan Command Buffer!");
        }

        VkCommandBufferBeginInfo CommandBufferBeginInfo{};
        CommandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        CommandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        if (vkBeginCommandBuffer(CommandBuffer, &CommandBufferBeginInfo) != VK_SUCCESS)
        {
            CORVUS_CORE_CRITICAL("Failed to begin Vulkan Command Buffer!");
        }

        return CommandBuffer;
    }

    void CRenderer::EndSingleTimeCommand(VkCommandBuffer CommandBuffer)
    {
        if (vkEndCommandBuffer(CommandBuffer) != VK_SUCCESS)
        {
            CORVUS_CORE_CRITICAL("Failed to end Vulkan Command Buffer!");
        }

        VkSubmitInfo SubmitInfo{};
        SubmitInfo.sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        SubmitInfo.commandBufferCount = 1;
        SubmitInfo.pCommandBuffers    = &CommandBuffer;

        if (vkQueueSubmit(m_Queues.GraphicsQueue, 1, &SubmitInfo, VK_NULL_HANDLE) != VK_SUCCESS)
        {
            CORVUS_CORE_CRITICAL("Failed to submit Vulkan Command Buffer!");
        }

        if (vkQueueWaitIdle(m_Queues.GraphicsQueue) != VK_SUCCESS)
        {
            CORVUS_CORE_CRITICAL("Vulkan Queue wait idle failed!");
        }

        vkFreeCommandBuffers(m_Device, m_CommandPool, 1, &CommandBuffer);
    }

} // namespace Corvus