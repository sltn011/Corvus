#ifndef CORVUS_SOURCE_CORVUS_RENDERER_RENDERPASS_RENDERPASS_POSTPROCESS_H
#define CORVUS_SOURCE_CORVUS_RENDERER_RENDERPASS_RENDERPASS_POSTPROCESS_H

#include "Corvus/Core/Base.h"
#include "Corvus/Renderer/Resources/RenderTarget.h"

#include <vulkan/vulkan.h>

namespace Corvus
{

    class CRenderPass_Postprocess
    {
    public:
        void Create();
        void Destroy();

        void Render(VkCommandBuffer CommandBuffer);

    public:
        VkRenderPass RenderPass = VK_NULL_HANDLE;
        VkPipeline   Pipeline   = VK_NULL_HANDLE;

    private:
        void CreateRenderPass();
        void CreatePipeline();
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_RENDERER_RENDERPASS_RENDERPASS_POSTPROCESS_H
