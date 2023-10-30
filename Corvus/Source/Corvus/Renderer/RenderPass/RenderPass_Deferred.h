#ifndef CORVUS_SOURCE_CORVUS_RENDERER_RENDERPASS_RENDERPASS_DEFERRED_H
#define CORVUS_SOURCE_CORVUS_RENDERER_RENDERPASS_RENDERPASS_DEFERRED_H

#include "Corvus/Core/Base.h"
#include "Corvus/Renderer/Resources/RenderTarget.h"

#include <vulkan/vulkan.h>

namespace Corvus
{

    class CRenderPass_Deferred
    {
    public:
        void Create();
        void Destroy();

        void BeginRender(VkCommandBuffer CommandBuffer);
        void EndRender(VkCommandBuffer CommandBuffer);

    public:
        VkRenderPass     RenderPass     = VK_NULL_HANDLE;
        VkPipelineLayout PipelineLayout = VK_NULL_HANDLE;
        VkPipeline       Pipeline       = VK_NULL_HANDLE;
        CRenderTarget    RenderTarget;

    private:
        void CreateRenderPass();
        void CreateLayout();
        void CreatePipeline();
        void CreateRenderTarget();
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_RENDERER_RENDERPASS_RENDERPASS_DEFERRED_H
