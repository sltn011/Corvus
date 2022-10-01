#include "CorvusPCH.h"

#include "Corvus/Assets/Model/StaticMeshPrimitive.h"

namespace Corvus
{
    CStaticMeshPrimitive::CStaticMeshPrimitive(TOwn<CVertexArray> &&VertexArray, CMaterial &&Material)
        : m_VertexArray{std::move(VertexArray)}, m_Material{std::move(Material)}
    {
    }

    TOwn<CVertexArray> &CStaticMeshPrimitive::GetVertexArray()
    {
        return m_VertexArray;
    }

    CMaterial &CStaticMeshPrimitive::GetMaterial()
    {
        return m_Material;
    }

} // namespace Corvus
