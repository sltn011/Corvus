#include "CorvusPCH.h"

#include "Corvus/Renderer/Renderer.h"

namespace Corvus
{

    CVulkanBuffer CRenderer::CreateBuffer(VkBufferUsageFlags Usage, VkDeviceSize Size, VkMemoryPropertyFlags Properties)
    {
        CVulkanBuffer Buffer;

        VkBufferCreateInfo BufferCreateInfo =
            VkInit::BufferCreateInfo(Usage, Size, VK_SHARING_MODE_EXCLUSIVE); // only will be used by graphics queue

        if (vkCreateBuffer(Device, &BufferCreateInfo, nullptr, &Buffer.Buffer) != VK_SUCCESS)
        {
            CORVUS_CORE_CRITICAL("Failed to create Vulkan Buffer!");
        }
        CORVUS_CORE_TRACE("Created Vulkan Buffer successfully");

        VkMemoryRequirements BufferMemoryRequirements{};
        vkGetBufferMemoryRequirements(Device, Buffer.Buffer, &BufferMemoryRequirements);

        VkMemoryAllocateInfo BufferMemoryAllocateInfo = VkInit::MemoryAllocateInfo(
            BufferMemoryRequirements.size, FindMemoryType(BufferMemoryRequirements.memoryTypeBits, Properties)
        );

        Buffer.Memory = AllocateDeviceMemory(BufferMemoryAllocateInfo);

        vkBindBufferMemory(Device, Buffer.Buffer, Buffer.Memory, 0);

        return Buffer;
    }

    void CRenderer::DestroyBuffer(CVulkanBuffer &Buffer)
    {
        if (Buffer.Buffer && Buffer.Memory)
        {
            vkDestroyBuffer(Device, Buffer.Buffer, nullptr);
            vkFreeMemory(Device, Buffer.Memory, nullptr);
            Buffer.Buffer = VK_NULL_HANDLE;
            Buffer.Memory = VK_NULL_HANDLE;
        }
    }

    void CRenderer::DestroyBuffer(CVulkanUniformBuffer &Buffer)
    {
        if (Buffer.Buffer && Buffer.Memory)
        {
            vkDestroyBuffer(Device, Buffer.Buffer, nullptr);
            vkFreeMemory(Device, Buffer.Memory, nullptr);
            Buffer.Buffer       = VK_NULL_HANDLE;
            Buffer.Memory       = VK_NULL_HANDLE;
            Buffer.MappedMemory = nullptr;
        }
    }

    void CRenderer::TransferBufferData(VkBuffer Source, VkBuffer Destination, VkDeviceSize Size)
    {
        VkCommandBuffer CommandBuffer = BeginSingleTimeCommand();

        VkBufferCopy TransferInfo = VkInit::BufferCopy(0, 0, Size);

        vkCmdCopyBuffer(CommandBuffer, Source, Destination, 1, &TransferInfo);

        EndSingleTimeCommand(CommandBuffer);
    }

    void CRenderer::TransferBufferData(VkBuffer Source, VkImage Destination, UInt32 Width, UInt32 Height)
    {
        VkCommandBuffer CommandBuffer = BeginSingleTimeCommand();

        VkBufferImageCopy Region = VkInit::BufferImageCopy({Width, Height, 1});

        vkCmdCopyBufferToImage(CommandBuffer, Source, Destination, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &Region);

        EndSingleTimeCommand(CommandBuffer);
    }

    void CRenderer::CreateUniformBuffers()
    {
        for (UInt32 i = 0; i < s_FramesInFlight; ++i)
        {
            CameraUBOs[i] = CreateUniformBuffer<CCameraUBO>();
        }
        for (UInt32 i = 0; i < s_FramesInFlight; ++i)
        {
            RenderTargetUBOs[i] = CreateUniformBuffer<CRenderTargetUBO>();
        }
        CORVUS_CORE_TRACE("Vulkan Uniform Buffers created");
    }

    void CRenderer::DestroyUniformBuffers()
    {
        if (!CameraUBOs.empty())
        {
            for (UInt32 i = 0; i < s_FramesInFlight; ++i)
            {
                DestroyBuffer(CameraUBOs[i]);
            }
        }
        if (!RenderTargetUBOs.empty())
        {
            for (UInt32 i = 0; i < s_FramesInFlight; ++i)
            {
                DestroyBuffer(RenderTargetUBOs[i]);
            }
        }
        CORVUS_CORE_TRACE("Vulkan Uniform Buffers destroyed");
    }

} // namespace Corvus
