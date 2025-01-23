#include "CorvusPCH.h"

#include "Corvus/Assets/Material/Material.h"

#include "Corvus/Renderer/Renderer.h"

namespace Corvus
{

    void CRenderer::CreateMaterialRenderData(CMaterial &Material)
    {
        std::vector<VkImageView> ImageViews      = {Material.Albedo.ImageView, Material.Normal.ImageView};
        std::vector<VkSampler>   TextureSamplers = {Material.Albedo.Sampler, Material.Normal.Sampler};

        VkDescriptorSet DescriptorSet = AllocateDescriptorSets<1>(PerDrawDescriptorPool, PerDrawDescriptorSetLayout)[0];
        ConfigureDescriptorSet<2>(Device, DescriptorSet, ImageViews.data(), TextureSamplers.data());

        Material.DescriptorSet = DescriptorSet;
    }

    void CRenderer::DestroyMaterialRenderData(CMaterial &Material)
    {
        FreeDescriptorSets(PerDrawDescriptorPool, &Material.DescriptorSet, 1);
    }
} // namespace Corvus
