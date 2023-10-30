#include "CorvusPCH.h"

#include "Corvus/Renderer/Resources/Attachment.h"

#include "Corvus/Renderer/Renderer.h"

namespace Corvus
{

    CAttachment CRenderer::CreateColorAttachment(VkFormat Format, VkExtent2D Extent)
    {
        CAttachment Attachment{};

        Attachment.Image = CreateImage(
            Extent.width,
            Extent.height,
            1,
            Format,
            VK_IMAGE_TILING_OPTIMAL,
            VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
        );

        Attachment.ImageView = CreateImageView(Attachment.Image.Image, 1, Format, VK_IMAGE_ASPECT_COLOR_BIT);

        Attachment.Format = Format;
        Attachment.Extent = Extent;

        return Attachment;
    }

    CAttachment CRenderer::CreateDepthStencilAttachment(VkFormat Format, VkExtent2D Extent)
    {
        CAttachment Attachment{};

        Attachment.Image = CreateImage(
            Extent.width,
            Extent.height,
            1,
            Format,
            VK_IMAGE_TILING_OPTIMAL,
            VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
        );

        Attachment.ImageView = CreateImageView(Attachment.Image.Image, 1, Format, VK_IMAGE_ASPECT_DEPTH_BIT);

        Attachment.Format = Format;
        Attachment.Extent = Extent;

        return Attachment;
    }

    void CRenderer::DestroyAttachment(CAttachment &Attachment)
    {
        DestroyImageView(Attachment.ImageView);
        DestroyImage(Attachment.Image);
    }

} // namespace Corvus
