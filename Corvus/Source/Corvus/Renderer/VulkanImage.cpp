#include "CorvusPCH.h"

#include "Corvus/Renderer/VulkanImage.h"

#include "Corvus/Assets/Texture/ImageData.h"
#include "Corvus/Renderer/Renderer.h"

namespace Corvus
{
    CVulkanImage CRenderer::CreateImage(
        UInt32                Width,
        UInt32                Height,
        VkFormat              Format,
        VkImageTiling         Tiling,
        VkImageUsageFlags     Usage,
        VkMemoryPropertyFlags Properties
    )
    {
        CVulkanImage Image;

        VkImageCreateInfo ImageCreateInfo{};
        ImageCreateInfo.sType         = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        ImageCreateInfo.imageType     = VK_IMAGE_TYPE_2D;
        ImageCreateInfo.extent.width  = Width;
        ImageCreateInfo.extent.height = Height;
        ImageCreateInfo.extent.depth  = 1;
        ImageCreateInfo.mipLevels     = 1;
        ImageCreateInfo.arrayLayers   = 1;
        ImageCreateInfo.format        = Format;
        ImageCreateInfo.tiling        = Tiling;
        ImageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        ImageCreateInfo.usage         = Usage;
        ImageCreateInfo.samples       = VK_SAMPLE_COUNT_1_BIT;
        ImageCreateInfo.sharingMode   = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateImage(m_Device, &ImageCreateInfo, nullptr, &Image.Image) != VK_SUCCESS)
        {
            CORVUS_CORE_CRITICAL("Failed to create Vulkan Image!");
        }

        VkMemoryRequirements ImageMemoryRequirements;
        vkGetImageMemoryRequirements(m_Device, Image.Image, &ImageMemoryRequirements);

        VkMemoryAllocateInfo MemoryAllocateInfo{};
        MemoryAllocateInfo.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        MemoryAllocateInfo.allocationSize  = ImageMemoryRequirements.size;
        MemoryAllocateInfo.memoryTypeIndex = FindMemoryType(ImageMemoryRequirements.memoryTypeBits, Properties);

        Image.Memory = AllocateDeviceMemory(MemoryAllocateInfo);

        vkBindImageMemory(m_Device, Image.Image, Image.Memory, 0);

        return Image;
    }

    CVulkanImage CRenderer::CreateTextureImage(CImageData const &ImageData)
    {
        VkDeviceSize ImageSize = ImageData.GetImageSize();

        CVulkanBuffer StagingBuffer = CreateBuffer(
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            ImageSize,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
        );

        void *StagingBufferMemory = MapDeviceMemory(StagingBuffer.Memory, ImageSize, 0);
        std::memcpy(StagingBufferMemory, ImageData.GetImageRawData(), ImageSize);
        UnmapDeviceMemory(StagingBuffer.Memory);

        CVulkanImage TextureImage = CreateImage(
            static_cast<UInt32>(ImageData.GetImageWidth()),
            static_cast<UInt32>(ImageData.GetImageHeight()),
            ImageData.GetPixelFormat(),
            VK_IMAGE_TILING_OPTIMAL,
            VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
        );

        TransitionImageLayout(
            TextureImage.Image,
            ImageData.GetPixelFormat(),
            VK_IMAGE_LAYOUT_UNDEFINED,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
        );
        TransferBufferData(
            StagingBuffer.Buffer,
            TextureImage.Image,
            static_cast<UInt32>(ImageData.GetImageWidth()),
            static_cast<UInt32>(ImageData.GetImageHeight())
        );
        TransitionImageLayout(
            TextureImage.Image,
            ImageData.GetPixelFormat(),
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        );

        DestroyBuffer(StagingBuffer);

        return TextureImage;
    }

    void CRenderer::DestroyImage(CVulkanImage &Image)
    {
        if (Image.Image != VK_NULL_HANDLE && Image.Memory != VK_NULL_HANDLE)
        {
            vkDestroyImage(m_Device, Image.Image, nullptr);
            vkFreeMemory(m_Device, Image.Memory, nullptr);
            Image.Image  = VK_NULL_HANDLE;
            Image.Memory = VK_NULL_HANDLE;
        }
    }

    void CRenderer::TransitionImageLayout(
        VkImage Image, VkFormat Format, VkImageLayout OldLayout, VkImageLayout NewLayout
    )
    {
        VkCommandBuffer CommandBuffer = BeginSingleTimeCommand();

        VkImageMemoryBarrier Barrier{};
        Barrier.sType                           = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        Barrier.oldLayout                       = OldLayout;
        Barrier.newLayout                       = NewLayout;
        Barrier.srcQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
        Barrier.dstQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
        Barrier.image                           = Image;
        Barrier.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
        Barrier.subresourceRange.baseMipLevel   = 0;
        Barrier.subresourceRange.levelCount     = 1;
        Barrier.subresourceRange.baseArrayLayer = 0;
        Barrier.subresourceRange.layerCount     = 1;

        VkPipelineStageFlags SourceStage;
        VkPipelineStageFlags DestinationStage;

        if (OldLayout == VK_IMAGE_LAYOUT_UNDEFINED && NewLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
        {
            Barrier.srcAccessMask = 0;
            Barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

            SourceStage      = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            DestinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        }
        else if (OldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && NewLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
        {
            Barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            Barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            SourceStage      = VK_PIPELINE_STAGE_TRANSFER_BIT;
            DestinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        }
        else if (OldLayout == VK_IMAGE_LAYOUT_UNDEFINED && NewLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
        {
            Barrier.srcAccessMask = 0;
            Barrier.dstAccessMask =
                VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

            SourceStage      = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            DestinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        }
        else
        {
            CORVUS_CORE_ERROR("Unsupported Vulkan Layout transition!");
        }

        vkCmdPipelineBarrier(CommandBuffer, SourceStage, DestinationStage, 0, 0, nullptr, 0, nullptr, 1, &Barrier);

        EndSingleTimeCommand(CommandBuffer);
    }

    VkFormat CRenderer::FindSupportedFormat(
        std::vector<VkFormat> const &Candidates, VkImageTiling Tiling, VkFormatFeatureFlags Features
    )
    {
        for (VkFormat Format : Candidates)
        {
            VkFormatProperties FormatProperties;
            vkGetPhysicalDeviceFormatProperties(m_PhysicalDevice, Format, &FormatProperties);

            if (Tiling == VK_IMAGE_TILING_OPTIMAL && (FormatProperties.optimalTilingFeatures & Features) == Features)
            {
                return Format;
            }
            else if (Tiling == VK_IMAGE_TILING_LINEAR && (FormatProperties.linearTilingFeatures & Features) == Features)
            {
                return Format;
            }

            CORVUS_CORE_CRITICAL("Could not find requested Vulkan Format!");
        }
    }

    bool CRenderer::bFormatSupportsStencilData(VkFormat Format)
    {
        return Format == VK_FORMAT_D32_SFLOAT_S8_UINT || Format == VK_FORMAT_D24_UNORM_S8_UINT;
    }

    void CRenderer::CreateDepthResources()
    {
        VkFormat DepthFormat = FindDepthFormat();

        m_DepthImage = CreateImage(
            m_SwapchainExtent.width,
            m_SwapchainExtent.height,
            DepthFormat,
            VK_IMAGE_TILING_OPTIMAL,
            VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
        );

        m_DepthImageView = CreateImageView(m_DepthImage.Image, DepthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);

        // Don't need to transition layout here, will be done in RenderPass
    }

    void CRenderer::DestroyDepthResources()
    {
        DestroyImageView(m_DepthImageView);
        DestroyImage(m_DepthImage);
    }

    VkFormat CRenderer::FindDepthFormat()
    {
        return FindSupportedFormat(
            {VK_FORMAT_D24_UNORM_S8_UINT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D32_SFLOAT},
            VK_IMAGE_TILING_OPTIMAL,
            VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
        );
    }

} // namespace Corvus
