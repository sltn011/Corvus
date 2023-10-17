#include "CorvusPCH.h"

#include "Corvus/Assets/Texture/Texture2D.h"

#include "Corvus/Assets/Texture/ImageData.h"
#include "Corvus/Renderer/Renderer.h"

namespace Corvus
{

    CTexture2D CRenderer::CreateTexture2D(CImageData const &ImageData, UInt32 MipLevels, VkSampler TextureSampler)
    {
        CVulkanImage Image = CreateTextureImage(ImageData, MipLevels);
        VkImageView  ImageView =
            CreateImageView(Image.Image, MipLevels, ImageData.GetPixelFormat(), VK_IMAGE_ASPECT_COLOR_BIT);

        CTexture2D Texture2D;
        Texture2D.Image     = Image;
        Texture2D.ImageView = ImageView;
        Texture2D.Sampler   = m_Samplers.DefaultSampler;

        return Texture2D;
    }

    void CRenderer::DestroyTexture2D(CTexture2D &Texture2D)
    {
        DestroyImageView(Texture2D.ImageView);
        DestroyImage(Texture2D.Image);
    }

} // namespace Corvus
