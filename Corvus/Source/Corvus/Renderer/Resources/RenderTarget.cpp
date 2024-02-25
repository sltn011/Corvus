#include "CorvusPCH.h"

#include "Corvus/Renderer/Resources/RenderTarget.h"

#include "Corvus/Renderer/Renderer.h"

namespace Corvus
{
    void ConfigureRenderTargetDescriptorSet(
        VkDevice           Device,
        VkDescriptorSet    DescriptorSet,
        CAttachment const *pAttachments,
        VkImageView const *pImageViews,
        VkSampler const   *pSamplers,
        SizeT              NumViews
    )
    {
        std::vector<VkWriteDescriptorSet>  DescriptorSetWrites(NumViews, VkWriteDescriptorSet{});
        std::vector<VkDescriptorImageInfo> DescriptorImageInfos(NumViews, VkDescriptorImageInfo{});
        for (SizeT i = 0; i < NumViews; ++i)
        {
            DescriptorImageInfos[i].imageLayout = pAttachments[i].Layout;
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

    CRenderTarget CRenderer::CreateRenderTarget(
        std::vector<CAttachment> Attachments,
        VkExtent2D               Extent,
        VkRenderPass             RenderPass,
        VkDescriptorSetLayout    DescriptorSetLayout
    )
    {
        std::vector<VkImageView> AttachmentViews(Attachments.size());
        for (SizeT i = 0; i < Attachments.size(); ++i)
        {
            AttachmentViews[i] = Attachments[i].ImageView;
        }

        std::vector<VkSampler> Samplers(Attachments.size(), Samplers.RepeatLinear_Linear);

        CRenderTarget RenderTarget{};
        RenderTarget.Framebuffer = CreateFramebuffer(RenderPass, Extent, 1, AttachmentViews);

        RenderTarget.DescriptorSet = AllocateDescriptorSets<1>(PerDrawDescriptorPool, DescriptorSetLayout)[0];
        ConfigureRenderTargetDescriptorSet(
            Device,
            RenderTarget.DescriptorSet,
            Attachments.data(),
            AttachmentViews.data(),
            Samplers.data(),
            AttachmentViews.size()
        );

        RenderTarget.Extent      = Extent;
        RenderTarget.Attachments = std::move(Attachments);

        return RenderTarget;
    }

    void CRenderer::DestroyRenderTarget(CRenderTarget &RenderTarget)
    {
        for (CAttachment &Attachment : RenderTarget.Attachments)
        {
            DestroyAttachment(Attachment);
        }
        DestroyFramebuffer(RenderTarget.Framebuffer);
    }

} // namespace Corvus
