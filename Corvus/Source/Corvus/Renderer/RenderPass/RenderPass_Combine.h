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

        void Render(VkCommandBuffer CommandBuffer, UInt32 SwapchainImageIndex);

    public:
        VkRenderPass     RenderPass     = VK_NULL_HANDLE;
        VkPipelineLayout PipelineLayout = VK_NULL_HANDLE;
        VkPipeline       Pipeline       = VK_NULL_HANDLE;

    private:
        void CreateRenderPass();
        void CreateLayout();
        void CreatePipeline();
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_RENDERER_RENDERPASS_RENDERPASS_DEFERRED_H
