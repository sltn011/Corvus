#ifndef CORVUS_SOURCE_CORVUS_RENDERER_RESOURCES_SCREENQUAD_H
#define CORVUS_SOURCE_CORVUS_RENDERER_RESOURCES_SCREENQUAD_H

#include "Corvus/Core/Base.h"
#include "Corvus/Renderer/Memory/VulkanBuffer.h"

#include <vulkan/vulkan.h>

namespace Corvus
{

    class CScreenQuad
    {
    public:
        void Create();
        void Destroy();

        CVulkanBuffer VertexBuffer;
        CVulkanBuffer IndexBuffer;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_RENDERER_RESOURCES_SCREENQUAD_H
