#ifndef CORVUS_SOURCE_CORVUS_RENDERER_RESOURCES_ATTACHMENT_H
#define CORVUS_SOURCE_CORVUS_RENDERER_RESOURCES_ATTACHMENT_H

#include "Corvus/Core/Base.h"
#include "Corvus/Renderer/Resources/VulkanImage.h"

#include <vulkan/vulkan.h>

namespace Corvus
{

    class CAttachment
    {
    public:
        CVulkanImage  Image;
        VkImageView   ImageView = VK_NULL_HANDLE;
        VkFormat      Format;
        VkExtent2D    Extent;
        VkImageLayout Layout;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_RENDERER_RESOURCES_ATTACHMENT_H
