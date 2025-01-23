#include "CorvusPCH.h"

#include "Corvus/Assets/Model/StaticMeshPrimitive.h"

#include "Corvus/Renderer/Resources/Shapes/BaseShape.h"

namespace Corvus
{

    CStaticMeshPrimitive CStaticMeshPrimitive::FromShape(CBaseShape &&Shape)
    {
        CStaticMeshPrimitive Primitive;
        Primitive.VertexBuffer = std::exchange(Shape.VertexBuffer, CVulkanBuffer{});
        Primitive.VertexData   = std::move(Shape.VertexData);
        Primitive.IndexBuffer  = std::exchange(Shape.IndexBuffer, CVulkanBuffer{});
        Primitive.IndexData    = std::move(Shape.IndexData);
        return Primitive;
    }

} // namespace Corvus
