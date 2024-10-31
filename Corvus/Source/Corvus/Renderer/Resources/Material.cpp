#include "CorvusPCH.h"

#include "Corvus/Assets/Material/Material.h"

#include "Corvus/Renderer/Renderer.h"

namespace Corvus
{
    template<SizeT TAmount>
    void ConfigureMaterialDescriptorSet(
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

    void CRenderer::CreateMaterialRenderData(CMaterial &Material)
    {
        std::vector<VkImageView> ImageViews      = {Material.Albedo.ImageView, Material.Normal.ImageView};
        std::vector<VkSampler>   TextureSamplers = {Material.Albedo.Sampler, Material.Normal.Sampler};

        VkDescriptorSet DescriptorSet = AllocateDescriptorSets<1>(PerDrawDescriptorPool, PerDrawDescriptorSetLayout)[0];
        ConfigureMaterialDescriptorSet<2>(Device, DescriptorSet, ImageViews.data(), TextureSamplers.data());

        Material.DescriptorSet = DescriptorSet;
    }

    void CRenderer::DestroyMaterialRenderData(CMaterial &Material)
    {
        FreeDescriptorSets(PerDrawDescriptorPool, &Material.DescriptorSet, 1);
    }
} // namespace Corvus
