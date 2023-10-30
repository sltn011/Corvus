#include "CorvusPCH.h"

#include "Corvus/Renderer/Renderer.h"

namespace Corvus
{

    VkImageView CRenderer::CreateImageView(
        VkImage Image, UInt32 MipLevels, VkFormat Format, VkImageAspectFlags AspectFlags
    )
    {
        VkImageView ImageView = VK_NULL_HANDLE;

        VkImageViewCreateInfo CreateInfo{};
        CreateInfo.sType    = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        CreateInfo.image    = Image;
        CreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        CreateInfo.format   = Format;

        CreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY; // Default mapping
        CreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        CreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        CreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

        CreateInfo.subresourceRange.aspectMask     = AspectFlags;
        CreateInfo.subresourceRange.baseMipLevel   = 0;
        CreateInfo.subresourceRange.levelCount     = MipLevels;
        CreateInfo.subresourceRange.baseArrayLayer = 0;
        CreateInfo.subresourceRange.layerCount     = 1;

        if (vkCreateImageView(Device, &CreateInfo, nullptr, &ImageView) != VK_SUCCESS)
        {
            CORVUS_CORE_CRITICAL("Failed to create Vulkan Image View!");
        }

        return ImageView;
    }

    void CRenderer::DestroyImageView(VkImageView &ImageView)
    {
        if (ImageView != VK_NULL_HANDLE)
        {
            vkDestroyImageView(Device, ImageView, nullptr);
            ImageView = VK_NULL_HANDLE;
        }
    }

} // namespace Corvus