#include "CorvusPCH.h"

#include "Corvus/Renderer/Resources/VulkanImage.h"

#include "Corvus/Assets/Texture/ImageData.h"
#include "Corvus/Renderer/Renderer.h"

namespace Corvus
{
    CVulkanImage CRenderer::CreateImage(
        UInt32                Width,
        UInt32                Height,
        UInt32                MipLevels,
        VkFormat              Format,
        VkImageTiling         Tiling,
        VkImageUsageFlags     Usage,
        VkMemoryPropertyFlags Properties
    )
    {
        CVulkanImage Image;

        VkImageCreateInfo ImageCreateInfo = VkInit::ImageCreateInfo(
            VkExtent3D{Width, Height, 1}, MipLevels, Format, Tiling, Usage, VK_SHARING_MODE_EXCLUSIVE
        );

        if (vkCreateImage(Device, &ImageCreateInfo, nullptr, &Image.Image) != VK_SUCCESS)
        {
            CORVUS_CORE_CRITICAL("Failed to create Vulkan Image!");
        }

        VkMemoryRequirements ImageMemoryRequirements;
        vkGetImageMemoryRequirements(Device, Image.Image, &ImageMemoryRequirements);

        VkMemoryAllocateInfo MemoryAllocateInfo = VkInit::MemoryAllocateInfo(
            ImageMemoryRequirements.size, FindMemoryType(ImageMemoryRequirements.memoryTypeBits, Properties)
        );

        Image.Memory = AllocateDeviceMemory(MemoryAllocateInfo);

        vkBindImageMemory(Device, Image.Image, Image.Memory, 0);

        Image.MipLevels = MipLevels;

        return Image;
    }

    CVulkanImage CRenderer::CreateTextureImage(CImageData const &ImageData, UInt32 MipLevels)
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

        VkMemoryPropertyFlags TextureFlags = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT |
                                             (MipLevels > 1 ? VK_IMAGE_USAGE_TRANSFER_SRC_BIT : 0);

        CVulkanImage TextureImage = CreateImage(
            static_cast<UInt32>(ImageData.GetImageWidth()),
            static_cast<UInt32>(ImageData.GetImageHeight()),
            MipLevels,
            ImageData.GetPixelFormat(),
            VK_IMAGE_TILING_OPTIMAL,
            TextureFlags,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
        );

        TransitionImageLayout(
            TextureImage.Image,
            MipLevels,
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
        if (MipLevels == 1)
        {
            TransitionImageLayout(
                TextureImage.Image,
                MipLevels,
                ImageData.GetPixelFormat(),
                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
            );
        }

        DestroyBuffer(StagingBuffer);

        if (MipLevels > 1)
        {
            GenerateMips(
                TextureImage.Image,
                TextureImage.MipLevels,
                static_cast<UInt32>(ImageData.GetImageWidth()),
                static_cast<UInt32>(ImageData.GetImageHeight()),
                ImageData.GetPixelFormat()
            );
        }

        return TextureImage;
    }

    void CRenderer::DestroyImage(CVulkanImage &Image)
    {
        if (Image.Image != VK_NULL_HANDLE && Image.Memory != VK_NULL_HANDLE)
        {
            vkDestroyImage(Device, Image.Image, nullptr);
            vkFreeMemory(Device, Image.Memory, nullptr);
            Image.Image  = VK_NULL_HANDLE;
            Image.Memory = VK_NULL_HANDLE;
        }
    }

    void CRenderer::TransitionImageLayout(
        VkImage Image, UInt32 MipLevels, VkFormat Format, VkImageLayout OldLayout, VkImageLayout NewLayout
    )
    {
        VkCommandBuffer CommandBuffer = BeginSingleTimeCommand();

        VkAccessFlags        SrcAccessMask = VK_ACCESS_NONE;
        VkAccessFlags        DstAccessMask = VK_ACCESS_NONE;
        VkPipelineStageFlags SrcStage      = VK_PIPELINE_STAGE_NONE;
        VkPipelineStageFlags DstStage      = VK_PIPELINE_STAGE_NONE;

        if (OldLayout == VK_IMAGE_LAYOUT_UNDEFINED && NewLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
        {
            SrcAccessMask = 0;
            DstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

            SrcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            DstStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        }
        else if (OldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && NewLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
        {
            SrcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            DstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            SrcStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
            DstStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        }
        else if (OldLayout == VK_IMAGE_LAYOUT_UNDEFINED && NewLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
        {
            SrcAccessMask = 0;
            DstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

            SrcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            DstStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        }
        else
        {
            CORVUS_CORE_ERROR("Unsupported Vulkan Layout transition!");
            SrcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            DstStage = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
        }

        VkImageMemoryBarrier Barrier =
            VkInit::ImageMemoryBarrier(Image, OldLayout, NewLayout, 0, MipLevels, SrcAccessMask, DstAccessMask);

        vkCmdPipelineBarrier(CommandBuffer, SrcStage, DstStage, 0, 0, nullptr, 0, nullptr, 1, &Barrier);

        EndSingleTimeCommand(CommandBuffer);
    }

    void CRenderer::GenerateMips(
        VkImage Image, UInt32 MipLevels, UInt32 ImageWidth, UInt32 ImageHeight, VkFormat Format
    )
    {
        VkFormatProperties FormatProperties{};
        vkGetPhysicalDeviceFormatProperties(m_PhysicalDevice, Format, &FormatProperties);
        if (!(FormatProperties.linearTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT))
        {
            CORVUS_CORE_CRITICAL("Selected Image Format does not support linear blitting");
        }

        VkCommandBuffer CommandBuffer = BeginSingleTimeCommand();

        VkImageMemoryBarrier Barrier = VkInit::ImageMemoryBarrier(
            Image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_UNDEFINED, 0, 1, VK_ACCESS_NONE, VK_ACCESS_NONE
        );

        Int32 MipWidth  = static_cast<Int32>(ImageWidth);
        Int32 MipHeight = static_cast<Int32>(ImageHeight);
        for (UInt32 i = 1; i < MipLevels; ++i)
        {
            Barrier.subresourceRange.baseMipLevel = i - 1;
            Barrier.oldLayout                     = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            Barrier.srcAccessMask                 = VK_ACCESS_TRANSFER_WRITE_BIT;
            Barrier.newLayout                     = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
            Barrier.dstAccessMask                 = VK_ACCESS_TRANSFER_READ_BIT;

            vkCmdPipelineBarrier(
                CommandBuffer,
                VK_PIPELINE_STAGE_TRANSFER_BIT,
                VK_PIPELINE_STAGE_TRANSFER_BIT,
                0,
                0,
                nullptr,
                0,
                nullptr,
                1,
                &Barrier
            );

            Int32 NextMipWidth  = MipWidth > 1 ? MipWidth / 2 : 1;
            Int32 NextMipHeight = MipHeight > 1 ? MipHeight / 2 : 1;

            VkImageBlit BlitParams = VkInit::ImageBlit(MipWidth, MipHeight, i - 1, NextMipWidth, NextMipHeight, i);

            vkCmdBlitImage(
                CommandBuffer,
                Image,
                VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                Image,
                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                1,
                &BlitParams,
                VK_FILTER_LINEAR
            );

            Barrier.oldLayout     = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
            Barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
            Barrier.newLayout     = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            Barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            vkCmdPipelineBarrier(
                CommandBuffer,
                VK_PIPELINE_STAGE_TRANSFER_BIT,
                VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                0,
                0,
                nullptr,
                0,
                nullptr,
                1,
                &Barrier
            );

            MipWidth  = NextMipWidth;
            MipHeight = NextMipHeight;
        }

        Barrier.subresourceRange.baseMipLevel = MipLevels - 1;
        Barrier.oldLayout                     = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        Barrier.srcAccessMask                 = VK_ACCESS_TRANSFER_WRITE_BIT;
        Barrier.newLayout                     = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        Barrier.dstAccessMask                 = VK_ACCESS_SHADER_READ_BIT;

        vkCmdPipelineBarrier(
            CommandBuffer,
            VK_PIPELINE_STAGE_TRANSFER_BIT,
            VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
            0,
            0,
            nullptr,
            0,
            nullptr,
            1,
            &Barrier
        );

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
        }

        CORVUS_CORE_CRITICAL("Could not find requested Vulkan Format!");
        return VK_FORMAT_UNDEFINED;
    }

    bool CRenderer::bFormatSupportsStencilData(VkFormat Format)
    {
        return Format == VK_FORMAT_D32_SFLOAT_S8_UINT || Format == VK_FORMAT_D24_UNORM_S8_UINT;
    }

    void CRenderer::CreateDepthResources()
    {
        VkFormat DepthFormat = FindDepthFormat();

        DepthImage = CreateImage(
            SwapchainExtent.width,
            SwapchainExtent.height,
            1,
            DepthFormat,
            VK_IMAGE_TILING_OPTIMAL,
            VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
        );

        DepthImageView = CreateImageView(DepthImage.Image, 1, DepthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);

        // Don't need to transition layout here, will be done in RenderPass
    }

    void CRenderer::DestroyDepthResources()
    {
        DestroyImageView(DepthImageView);
        DestroyImage(DepthImage);
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
