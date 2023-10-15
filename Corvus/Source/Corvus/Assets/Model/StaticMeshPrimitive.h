#ifndef CORVUS_SOURCE_CORVUS_ASSETS_MODEL_STATICMESHPRIMITIVE_H
#define CORVUS_SOURCE_CORVUS_ASSETS_MODEL_STATICMESHPRIMITIVE_H

#include "Corvus/Assets/AssetRef.h"
// #include "Corvus/Assets/Material/Material.h"
#include "Corvus/Assets/Texture/Texture2D.h"
#include "Corvus/Renderer/Data/Vertex.h"
#include "Corvus/Renderer/VulkanBuffer.h"

namespace Corvus
{

    class CStaticMeshPrimitive
    {
    public:
        CStaticMeshPrimitive(
            CVulkanBuffer          PrimitiveVertexBuffer,
            std::vector<CVertex> &&PrimitiveVertexData,
            CVulkanBuffer          PrimitiveIndexBuffer,
            std::vector<UInt16>  &&PrimitiveIndexData
        );

    public:
        CVulkanBuffer        VertexBuffer;
        std::vector<CVertex> VertexData;

        CVulkanBuffer       IndexBuffer;
        std::vector<UInt16> IndexData;

        CTexture2D Texture2D;

        // TAssetRef<CMaterial> MaterialRef;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_ASSETS_MODEL_STATICMESHPRIMITIVE_H
