#include "CorvusPCH.h"

#include "Corvus/Assets/Texture/Texture.h"

#include "Corvus/Assets/Texture/ImageData.h"
#include "Corvus/Renderer/Renderer.h"

namespace Corvus
{

    CTexture CRenderer::CreateTexture(CImageData const &ImageData, UInt32 MipLevels, VkSampler TextureSampler)
    {
        CVulkanImage Image     = CreateTextureImage(ImageData, MipLevels);
        VkImageView  ImageView = CreateImageView(
            Image.Image, VK_IMAGE_VIEW_TYPE_2D, MipLevels, ImageData.GetPixelFormat(), VK_IMAGE_ASPECT_COLOR_BIT
        );

        CTexture Texture;
        Texture.Image     = Image;
        Texture.ImageView = ImageView;
        Texture.Sampler   = TextureSampler;

        return Texture;
    }

    void CRenderer::DestroyTexture(CTexture &Texture)
    {
        DestroyImageView(Texture.ImageView);
        DestroyImage(Texture.Image);
    }

} // namespace Corvus
