#ifndef CORVUS_SOURCE_CORVUS_RENDERER_RENDERPASS_RENDERPASS_COMBINE_H
#define CORVUS_SOURCE_CORVUS_RENDERER_RENDERPASS_RENDERPASS_COMBINE_H

#include "Corvus/Core/Base.h"
#include "Corvus/Renderer/Resources/RenderTarget.h"

#include <vulkan/vulkan.h>

namespace Corvus
{

    class CRenderPass_Combine
    {
    public:
        void Create();
        void Destroy();

        void Render(VkCommandBuffer CommandBuffer);

    public:
        VkRenderPass                         RenderPass = VK_NULL_HANDLE;
        VkPipeline                           Pipeline   = VK_NULL_HANDLE;
        CRenderTarget                        RenderTarget;
        std::vector<VkAttachmentDescription> AttachmentsDescriptions;

    private:
        void CreateRenderPass();
        void CreatePipeline();
        void CreateRenderTarget();
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_RENDERER_RENDERPASS_RENDERPASS_DEFERRED_H
