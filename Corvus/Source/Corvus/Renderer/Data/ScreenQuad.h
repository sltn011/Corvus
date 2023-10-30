#ifndef CORVUS_SOURCE_CORVUS_RENDERER_DATA_SCREENQUAD_H
#define CORVUS_SOURCE_CORVUS_RENDERER_DATA_SCREENQUAD_H

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

        static CScreenQuad &Get();

        CVulkanBuffer VertexBuffer;
        CVulkanBuffer IndexBuffer;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_RENDERER_DATA_SCREENQUAD_H
