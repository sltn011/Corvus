
namespace Corvus
{

    template<typename TVertex>
    CVulkanBuffer CRenderer::CreateVertexBuffer(std::vector<TVertex> const &BufferData)
    {
        VkDeviceSize BufferSize = static_cast<VkDeviceSize>(sizeof(BufferData[0]) * BufferData.size());

        CVulkanBuffer StagingBuffer = CreateBuffer(
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            BufferSize,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
        );

        void *StagingBufferData = MapDeviceMemory(StagingBuffer.Memory, BufferSize, 0);
        std::memcpy(StagingBufferData, BufferData.data(), static_cast<SizeT>(BufferSize));
        UnmapDeviceMemory(StagingBuffer.Memory);

        CVulkanBuffer VertexBuffer = CreateBuffer(
            VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
            BufferSize,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
        );

        TransferBufferData(StagingBuffer.Buffer, VertexBuffer.Buffer, BufferSize);

        DestroyBuffer(StagingBuffer);

        return VertexBuffer;
    }

    template<typename TIndex>
    CVulkanBuffer CRenderer::CreateIndexBuffer(std::vector<TIndex> const &BufferData)
    {
        VkDeviceSize BufferSize = static_cast<VkDeviceSize>(sizeof(BufferData[0]) * BufferData.size());

        CVulkanBuffer StagingBuffer = CreateBuffer(
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            BufferSize,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
        );

        void *StagingBufferData = MapDeviceMemory(StagingBuffer.Memory, BufferSize, 0);
        std::memcpy(StagingBufferData, BufferData.data(), static_cast<SizeT>(BufferSize));
        UnmapDeviceMemory(StagingBuffer.Memory);

        CVulkanBuffer IndexBuffer = CreateBuffer(
            VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
            BufferSize,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
        );

        TransferBufferData(StagingBuffer.Buffer, IndexBuffer.Buffer, BufferSize);

        DestroyBuffer(StagingBuffer);

        return IndexBuffer;
    }

    template<typename TUBO>
    CVulkanUniformBuffer CRenderer::CreateUniformBuffer()
    {
        static constexpr VkDeviceSize UBOSize = sizeof(TUBO);

        CVulkanBuffer UnmappedBuffer = CreateBuffer(
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            UBOSize,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
        );

        CVulkanUniformBuffer UniformBuffer;
        UniformBuffer.Buffer = UnmappedBuffer.Buffer;
        UniformBuffer.Memory = UnmappedBuffer.Memory;

        UniformBuffer.MappedMemory = MapDeviceMemory(UniformBuffer.Memory, UBOSize, 0);

        return UniformBuffer;
    }

} // namespace Corvus
