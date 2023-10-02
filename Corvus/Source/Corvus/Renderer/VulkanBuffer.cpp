#include "CorvusPCH.h"

#include "Corvus/Renderer/Renderer.h"

namespace Corvus
{

    CVulkanBuffer CRenderer::CreateBuffer(VkBufferUsageFlags Usage, VkDeviceSize Size, VkMemoryPropertyFlags Properties)
    {
        CVulkanBuffer Buffer;

        VkBufferCreateInfo BufferCreateInfo{};
        BufferCreateInfo.sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        BufferCreateInfo.usage       = Usage;
        BufferCreateInfo.size        = Size;
        BufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE; // only will be used by graphics queue

        if (vkCreateBuffer(m_Device, &BufferCreateInfo, nullptr, &Buffer.Buffer) != VK_SUCCESS)
        {
            CORVUS_CORE_CRITICAL("Failed to create Vulkan Buffer!");
        }
        CORVUS_CORE_TRACE("Created Vulkan Buffer successfully");

        VkMemoryRequirements BufferMemoryRequirements{};
        vkGetBufferMemoryRequirements(m_Device, Buffer.Buffer, &BufferMemoryRequirements);

        VkMemoryAllocateInfo BufferMemoryAllocateInfo{};
        BufferMemoryAllocateInfo.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        BufferMemoryAllocateInfo.allocationSize  = BufferMemoryRequirements.size;
        BufferMemoryAllocateInfo.memoryTypeIndex = FindMemoryType(BufferMemoryRequirements.memoryTypeBits, Properties);

        Buffer.Memory = AllocateDeviceMemory(BufferMemoryAllocateInfo);

        vkBindBufferMemory(m_Device, Buffer.Buffer, Buffer.Memory, 0);

        return Buffer;
    }

    void CRenderer::DestroyBuffer(CVulkanBuffer &Buffer)
    {
        if (Buffer.Buffer && Buffer.Memory)
        {
            vkDestroyBuffer(m_Device, Buffer.Buffer, nullptr);
            vkFreeMemory(m_Device, Buffer.Memory, nullptr);
            Buffer.Buffer = VK_NULL_HANDLE;
            Buffer.Memory = VK_NULL_HANDLE;
        }
    }

    void CRenderer::DestroyBuffer(CVulkanUniformBuffer &Buffer)
    {
        if (Buffer.Buffer && Buffer.Memory)
        {
            vkDestroyBuffer(m_Device, Buffer.Buffer, nullptr);
            vkFreeMemory(m_Device, Buffer.Memory, nullptr);
            Buffer.Buffer       = VK_NULL_HANDLE;
            Buffer.Memory       = VK_NULL_HANDLE;
            Buffer.MappedMemory = nullptr;
        }
    }

    void CRenderer::TransferBufferData(VkBuffer Source, VkBuffer Destination, VkDeviceSize Size)
    {
        VkCommandBufferAllocateInfo CommandBufferAllocateInfo{};
        CommandBufferAllocateInfo.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        CommandBufferAllocateInfo.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        CommandBufferAllocateInfo.commandPool        = m_TransferCommandPool;
        CommandBufferAllocateInfo.commandBufferCount = 1;

        VkCommandBuffer TransferCommandBuffer;
        if (vkAllocateCommandBuffers(m_Device, &CommandBufferAllocateInfo, &TransferCommandBuffer) != VK_SUCCESS)
        {
            CORVUS_CORE_CRITICAL("Failed to create transfer command buffer!");
        }

        VkCommandBufferBeginInfo TransferBeginInfo{};
        TransferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        TransferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        vkBeginCommandBuffer(TransferCommandBuffer, &TransferBeginInfo);

        VkBufferCopy TransferInfo{};
        TransferInfo.srcOffset = 0;
        TransferInfo.dstOffset = 0;
        TransferInfo.size      = Size;
        vkCmdCopyBuffer(TransferCommandBuffer, Source, Destination, 1, &TransferInfo);

        vkEndCommandBuffer(TransferCommandBuffer);

        VkSubmitInfo TransferSubmitInfo{};
        TransferSubmitInfo.sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        TransferSubmitInfo.commandBufferCount = 1;
        TransferSubmitInfo.pCommandBuffers    = &TransferCommandBuffer;
        vkQueueSubmit(m_Queues.GraphicsQueue, 1, &TransferSubmitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(m_Queues.GraphicsQueue);

        vkFreeCommandBuffers(m_Device, m_TransferCommandPool, 1, &TransferCommandBuffer);
    }

    void CRenderer::CreateUniformBuffers()
    {
        for (UInt32 i = 0; i < s_FramesInFlight; ++i)
        {
            m_MatricesUBOs[i] = CreateUniformBuffer<CMVPUBO>();
        }
        CORVUS_CORE_TRACE("Vulkan Uniform Buffers created");
    }

    void CRenderer::DestroyUniformBuffers()
    {
        if (!m_MatricesUBOs.empty())
        {
            for (UInt32 i = 0; i < s_FramesInFlight; ++i)
            {
                DestroyBuffer(m_MatricesUBOs[i]);
            }
            CORVUS_CORE_TRACE("Vulkan Uniform Buffers destroyed");
        }
    }

} // namespace Corvus
