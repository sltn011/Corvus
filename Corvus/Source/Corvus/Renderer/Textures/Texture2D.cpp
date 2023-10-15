#include "CorvusPCH.h"

#include "Corvus/Assets/Texture/Texture2D.h"

#include "Corvus/Assets/Texture/ImageData.h"
#include "Corvus/Renderer/Renderer.h"

namespace Corvus
{
    template<SizeT TAmount>
    void ConfigureTexture2DDescriptorSet(
        VkDevice Device, VkDescriptorSet DescriptorSet, VkImageView *pImageViews, VkSampler *pSamplers
    )
    {
        std::array<VkWriteDescriptorSet, TAmount>  DescriptorSetWrites{};
        std::array<VkDescriptorImageInfo, TAmount> DescriptorImageInfos{};
        for (SizeT i = 0; i < TAmount; ++i)
        {
            DescriptorImageInfos[i].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            DescriptorImageInfos[i].imageView   = pImageViews[i];
            DescriptorImageInfos[i].sampler     = pSamplers[i];

            DescriptorSetWrites[i].sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            DescriptorSetWrites[i].dstSet           = DescriptorSet;
            DescriptorSetWrites[i].dstBinding       = static_cast<UInt32>(i);
            DescriptorSetWrites[i].dstArrayElement  = 0;
            DescriptorSetWrites[i].descriptorType   = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            DescriptorSetWrites[i].descriptorCount  = 1;
            DescriptorSetWrites[i].pBufferInfo      = nullptr;
            DescriptorSetWrites[i].pImageInfo       = &DescriptorImageInfos[i];
            DescriptorSetWrites[i].pTexelBufferView = nullptr;
        }

        vkUpdateDescriptorSets(
            Device, static_cast<UInt32>(DescriptorSetWrites.size()), DescriptorSetWrites.data(), 0, nullptr
        );
    }

    CTexture2D CRenderer::CreateTexture2D(CImageData const &ImageData, VkSampler TextureSampler)
    {
        CVulkanImage Image     = CreateTextureImage(ImageData);
        VkImageView  ImageView = CreateImageView(Image.Image, ImageData.GetPixelFormat(), VK_IMAGE_ASPECT_COLOR_BIT);
        VkSampler    Sampler   = TextureSampler;

        VkDescriptorSet DescriptorSet =
            AllocateDescriptorSets<1>(m_PerDrawDescriptorPool, m_PerDrawDescriptorSetLayout)[0];
        ConfigureTexture2DDescriptorSet<1>(m_Device, DescriptorSet, &ImageView, &Sampler);

        CTexture2D Texture2D;
        Texture2D.Image         = Image;
        Texture2D.ImageView     = ImageView;
        Texture2D.Sampler       = m_Samplers.DefaultSampler;
        Texture2D.DescriptorSet = DescriptorSet;

        return Texture2D;
    }

    void CRenderer::DestroyTexture2D(CTexture2D &Texture2D)
    {
        DestroyImage(Texture2D.Image);
        DestroyImageView(Texture2D.ImageView);
        FreeDescriptorSets(m_PerDrawDescriptorPool, &Texture2D.DescriptorSet, 1);
    }

} // namespace Corvus
