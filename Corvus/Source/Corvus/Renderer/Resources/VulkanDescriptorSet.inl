
namespace Corvus
{
    template<SizeT TAmount>
    std::array<VkDescriptorSet, TAmount> CRenderer::AllocateDescriptorSets(
        VkDescriptorPool Pool, VkDescriptorSetLayout Layout
    )
    {
        std::array<VkDescriptorSet, TAmount> DescriptorSets{VK_NULL_HANDLE};

        std::array<VkDescriptorSetLayout, TAmount> SetsLayouts{};
        SetsLayouts.fill(Layout);

        VkDescriptorSetAllocateInfo DescriptorSetInfo =
            VkInit::DescriptorSetAllocateInfo(Pool, SetsLayouts.data(), SetsLayouts.size());

        if (vkAllocateDescriptorSets(Device, &DescriptorSetInfo, DescriptorSets.data()) != VK_SUCCESS)
        {
            CORVUS_CORE_CRITICAL("Failed to allocate Vulkan Descriptor Sets!");
        }
        CORVUS_CORE_TRACE("Allocated Vulkan Descriptor Sets successfully");

        return DescriptorSets;
    }

    template<SizeT TAmount>
    void CRenderer::ConfigureDescriptorSet(
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

} // namespace Corvus
