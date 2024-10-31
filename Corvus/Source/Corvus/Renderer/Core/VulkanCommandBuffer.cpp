#include "CorvusPCH.h"

#include "Corvus/Renderer/Renderer.h"

namespace Corvus
{

    void CRenderer::AllocateCommandBuffers()
    {
        VkCommandBufferAllocateInfo CommandBufferInfo =
            VkInit::CommandBufferAllocateInfo(CommandPool, static_cast<UInt32>(CommandBuffers.size()));

        if (vkAllocateCommandBuffers(Device, &CommandBufferInfo, CommandBuffers.data()) != VK_SUCCESS)
        {
            CORVUS_CORE_CRITICAL("Failed to allocate Vulkan Command Buffer!");
        }
        CORVUS_CORE_TRACE("Allocated Vulkan Command Buffer successfully");
    }

    VkCommandBuffer CRenderer::BeginSingleTimeCommand()
    {
        VkCommandBufferAllocateInfo BufferAllocateInfo = VkInit::CommandBufferAllocateInfo(CommandPool, 1);

        VkCommandBuffer CommandBuffer;
        if (vkAllocateCommandBuffers(Device, &BufferAllocateInfo, &CommandBuffer) != VK_SUCCESS)
        {
            CORVUS_CORE_CRITICAL("Failed to allocate Vulkan Command Buffer!");
        }

        VkCommandBufferBeginInfo CommandBufferBeginInfo =
            VkInit::CommandBufferBeginInfo(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

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

        VkSubmitInfo SubmitInfo = VkInit::SubmitInfo(CommandBuffer, nullptr, 0, nullptr, nullptr, 0);

        if (vkQueueSubmit(Queues.GraphicsQueue, 1, &SubmitInfo, VK_NULL_HANDLE) != VK_SUCCESS)
        {
            CORVUS_CORE_CRITICAL("Failed to submit Vulkan Command Buffer!");
        }

        if (vkQueueWaitIdle(Queues.GraphicsQueue) != VK_SUCCESS)
        {
            CORVUS_CORE_CRITICAL("Vulkan Queue wait idle failed!");
        }

        vkFreeCommandBuffers(Device, CommandPool, 1, &CommandBuffer);
    }

} // namespace Corvus
