#ifndef CORVUS_SOURCE_CORVUS_RENDERER_RESOURCES_SHAPES_BASESHAPE_H
#define CORVUS_SOURCE_CORVUS_RENDERER_RESOURCES_SHAPES_BASESHAPE_H

#include "Corvus/Core/Base.h"
#include "Corvus/Renderer/Data/Vertex.h"
#include "Corvus/Renderer/Memory/VulkanBuffer.h"

#include <vulkan/vulkan.h>

namespace Corvus
{

    class CBaseShape
    {
    public:
        virtual void Create()  = 0;
        virtual void Destroy() = 0;

        CVulkanBuffer        VertexBuffer;
        std::vector<CVertex> VertexData;

        CVulkanBuffer       IndexBuffer;
        std::vector<UInt16> IndexData;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_RENDERER_RESOURCES_SHAPES_BASESHAPE_H
