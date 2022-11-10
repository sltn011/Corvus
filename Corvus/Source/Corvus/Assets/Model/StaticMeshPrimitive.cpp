#include "CorvusPCH.h"

#include "Corvus/Assets/Model/StaticMeshPrimitive.h"

namespace Corvus
{
    CStaticMeshPrimitive::CStaticMeshPrimitive(TOwn<CVertexArray> &&PrimitiveVertexArray)
        : VertexArray{std::move(PrimitiveVertexArray)}
    {
    }

} // namespace Corvus
