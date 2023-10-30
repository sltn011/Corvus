
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

        VkDescriptorSetAllocateInfo DescriptorSetInfo{};
        DescriptorSetInfo.sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        DescriptorSetInfo.descriptorPool     = Pool;
        DescriptorSetInfo.descriptorSetCount = static_cast<UInt32>(SetsLayouts.size());
        DescriptorSetInfo.pSetLayouts        = SetsLayouts.data();

        if (vkAllocateDescriptorSets(Device, &DescriptorSetInfo, DescriptorSets.data()) != VK_SUCCESS)
        {
            CORVUS_CORE_CRITICAL("Failed to allocate Vulkan Descriptor Sets!");
        }
        CORVUS_CORE_TRACE("Allocated Vulkan Descriptor Sets successfully");

        return DescriptorSets;
    }

} // namespace Corvus
