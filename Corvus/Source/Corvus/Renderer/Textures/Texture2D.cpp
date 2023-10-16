#include "CorvusPCH.h"

#include "Corvus/Assets/Texture/Texture2D.h"

#include "Corvus/Assets/Texture/ImageData.h"
#include "Corvus/Renderer/Renderer.h"

namespace Corvus
{

    CTexture2D CRenderer::CreateTexture2D(CImageData const &ImageData, VkSampler TextureSampler)
    {
        CVulkanImage Image     = CreateTextureImage(ImageData);
        VkImageView  ImageView = CreateImageView(Image.Image, ImageData.GetPixelFormat(), VK_IMAGE_ASPECT_COLOR_BIT);
        VkSampler    Sampler   = TextureSampler;

        CTexture2D Texture2D;
        Texture2D.Image     = Image;
        Texture2D.ImageView = ImageView;
        Texture2D.Sampler   = m_Samplers.DefaultSampler;

        return Texture2D;
    }

    void CRenderer::DestroyTexture2D(CTexture2D &Texture2D)
    {
        DestroyImage(Texture2D.Image);
        DestroyImageView(Texture2D.ImageView);
    }

} // namespace Corvus