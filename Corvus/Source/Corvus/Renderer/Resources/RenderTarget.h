#ifndef CORVUS_SOURCE_CORVUS_RENDERER_RESOURCES_RENDERTARGET_H
#define CORVUS_SOURCE_CORVUS_RENDERER_RESOURCES_RENDERTARGET_H

#include "Corvus/Core/Base.h"
#include "Corvus/Renderer/Resources/Attachment.h"

#include <vulkan/vulkan.h>

namespace Corvus
{

    class CRenderTarget
    {
    public:
        VkFramebuffer   Framebuffer;
        VkExtent2D      Extent;
        VkDescriptorSet DescriptorSet;

        std::vector<CAttachment> Attachments;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_RENDERER_RESOURCES_RENDERTARGET_H
