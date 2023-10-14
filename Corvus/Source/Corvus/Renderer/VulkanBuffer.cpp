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
        VkCommandBuffer CommandBuffer = BeginSingleTimeCommand();

        VkBufferCopy TransferInfo{};
        TransferInfo.srcOffset = 0;
        TransferInfo.dstOffset = 0;
        TransferInfo.size      = Size;
        vkCmdCopyBuffer(CommandBuffer, Source, Destination, 1, &TransferInfo);

        EndSingleTimeCommand(CommandBuffer);
    }

    void CRenderer::TransferBufferData(VkBuffer Source, VkImage Destination, UInt32 Width, UInt32 Height)
    {
        VkCommandBuffer CommandBuffer = BeginSingleTimeCommand();

        VkBufferImageCopy Region{};
        Region.bufferOffset                    = 0;
        Region.bufferRowLength                 = 0;
        Region.bufferImageHeight               = 0;
        Region.imageSubresource.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
        Region.imageSubresource.mipLevel       = 0;
        Region.imageSubresource.baseArrayLayer = 0;
        Region.imageSubresource.layerCount     = 1;
        Region.imageOffset                     = VkOffset3D{0, 0, 0};
        Region.imageExtent                     = VkExtent3D{Width, Height, 1};

        vkCmdCopyBufferToImage(CommandBuffer, Source, Destination, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &Region);

        EndSingleTimeCommand(CommandBuffer);
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
