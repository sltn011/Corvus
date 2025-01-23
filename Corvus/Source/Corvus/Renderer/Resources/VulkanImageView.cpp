#include "CorvusPCH.h"

#include "Corvus/Renderer/Renderer.h"

namespace Corvus
{

    VkImageView CRenderer::CreateImageView(
        VkImage            Image,
        VkImageViewType    ViewType,
        UInt32             MipLevels,
        VkFormat           Format,
        VkImageAspectFlags AspectFlags,
        UInt32             LayerCount
    )
    {
        VkImageView ImageView = VK_NULL_HANDLE;

        VkImageViewCreateInfo CreateInfo =
            VkInit::ImageViewCreateInfo(Image, Format, ViewType, AspectFlags, MipLevels, LayerCount);

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