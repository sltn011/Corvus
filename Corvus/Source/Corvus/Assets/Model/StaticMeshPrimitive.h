#ifndef CORVUS_SOURCE_CORVUS_ASSETS_MODEL_STATICMESHPRIMITIVE_H
#define CORVUS_SOURCE_CORVUS_ASSETS_MODEL_STATICMESHPRIMITIVE_H

#include "Corvus/Assets/AssetRef.h"
#include "Corvus/Assets/Material/Material.h"
#include "Corvus/Renderer/Data/Vertex.h"
#include "Corvus/Renderer/Memory/VulkanBuffer.h"

namespace Corvus
{

    struct CStaticMeshPrimitive
    {
        CVulkanBuffer        VertexBuffer;
        std::vector<CVertex> VertexData;

        CVulkanBuffer       IndexBuffer;
        std::vector<UInt16> IndexData;

        CMaterial Material;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_ASSETS_MODEL_STATICMESHPRIMITIVE_H
