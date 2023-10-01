#include "CorvusPCH.h"

#include "Corvus/Assets/Model/StaticMeshPrimitive.h"

namespace Corvus
{
    CStaticMeshPrimitive::CStaticMeshPrimitive(
        CVulkanBuffer          PrimitiveVertexBuffer,
        std::vector<CVertex> &&PrimitiveVertexData,
        CVulkanBuffer          PrimitiveIndexBuffer,
        std::vector<UInt16>  &&PrimitiveIndexData
    )
        : VertexBuffer{PrimitiveVertexBuffer},
          VertexData{std::move(PrimitiveVertexData)},
          IndexBuffer{PrimitiveIndexBuffer},
          IndexData{std::move(PrimitiveIndexData)}
    {
    }

} // namespace Corvus
