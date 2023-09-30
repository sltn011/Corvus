#include "CorvusPCH.h"

#include "Corvus/Renderer/Renderer.h"

namespace Corvus
{

    void CRenderer::CreateBuffer(
        VkBuffer             &Buffer,
        VkDeviceMemory       &BufferMemory,
        VkBufferUsageFlags    Usage,
        VkDeviceSize          Size,
        VkMemoryPropertyFlags Properties
    )
    {
        VkBufferCreateInfo BufferCreateInfo{};
        BufferCreateInfo.sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        BufferCreateInfo.usage       = Usage;
        BufferCreateInfo.size        = Size;
        BufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE; // only will be used by graphics queue

        if (vkCreateBuffer(m_Device, &BufferCreateInfo, nullptr, &Buffer) != VK_SUCCESS)
        {
            CORVUS_CRITICAL("Failed to create Vulkan Buffer!");
        }
        CORVUS_TRACE("Created Vulkan Buffer successfully");

        VkMemoryRequirements BufferMemoryRequirements{};
        vkGetBufferMemoryRequirements(m_Device, Buffer, &BufferMemoryRequirements);

        VkPhysicalDeviceMemoryProperties PhysicalDeviceMemoryProperties{};
        vkGetPhysicalDeviceMemoryProperties(m_PhysicalDevice, &PhysicalDeviceMemoryProperties);

        auto FindMemoryType = [PhysicalDeviceMemoryProperties](uint32_t TypeFilter, VkMemoryPropertyFlags Properties)
        {
            for (uint32_t i = 0; i < PhysicalDeviceMemoryProperties.memoryTypeCount; ++i)
            {
                if ((TypeFilter & (1 << i)) &&
                    (Properties & PhysicalDeviceMemoryProperties.memoryTypes[i].propertyFlags) == Properties)
                {
                    return i;
                }
            }

            CORVUS_CRITICAL("Required memory block not found for Vulkan Buffer!");
        };

        VkMemoryAllocateInfo BufferMemoryAllocateInfo{};
        BufferMemoryAllocateInfo.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        BufferMemoryAllocateInfo.allocationSize  = BufferMemoryRequirements.size;
        BufferMemoryAllocateInfo.memoryTypeIndex = FindMemoryType(BufferMemoryRequirements.memoryTypeBits, Properties);

        if (vkAllocateMemory(m_Device, &BufferMemoryAllocateInfo, nullptr, &BufferMemory) != VK_SUCCESS)
        {
            CORVUS_CRITICAL("Failed to allocate Vulkan Device Memory!");
        }
        CORVUS_TRACE("Allocated Vulkan Device Memory {0}KB successfully", BufferMemoryRequirements.size / 1000.f);

        vkBindBufferMemory(m_Device, Buffer, BufferMemory, 0);
    }

    void CRenderer::DestroyBuffer(VkBuffer &Buffer, VkDeviceMemory &BufferMemory)
    {
        if (Buffer && BufferMemory)
        {
            vkDestroyBuffer(m_Device, Buffer, nullptr);
            vkFreeMemory(m_Device, BufferMemory, nullptr);
            Buffer       = VK_NULL_HANDLE;
            BufferMemory = VK_NULL_HANDLE;
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
            CORVUS_CRITICAL("Failed to create transfer command buffer!");
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
        vkQueueSubmit(m_Queues.m_GraphicsQueue, 1, &TransferSubmitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(m_Queues.m_GraphicsQueue);

        vkFreeCommandBuffers(m_Device, m_TransferCommandPool, 1, &TransferCommandBuffer);
    }

} // namespace Corvus
