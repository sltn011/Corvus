#include "CorvusPCH.h"

#include "Corvus/Assets/Model/StaticMeshPrimitive.h"

namespace Corvus
{
    CStaticMeshPrimitive::CStaticMeshPrimitive(TOwn<CVertexArray> &&VertexArray) : m_VertexArray{std::move(VertexArray)}
    {
    }

    TOwn<CVertexArray> const &CStaticMeshPrimitive::GetVertexArray() const
    {
        return m_VertexArray;
    }

} // namespace Corvus
