#include "CorvusPCH.h"

#include "Corvus/Assets/Model/StaticMesh.h"

namespace Corvus
{
    CStaticMesh::CStaticMesh(FTransform const &Transform) : m_Transform{Transform}
    {
    }

    SizeT CStaticMesh::AddPrimitive(CStaticMeshPrimitive &&Primitive)
    {
        SizeT PrimitiveIndex = GetNumPrimitives();
        m_Primitives.emplace_back(std::move(Primitive));
        return PrimitiveIndex;
    }

    SizeT CStaticMesh::GetNumPrimitives() const
    {
        return m_Primitives.size();
    }

    CStaticMeshPrimitive &CStaticMesh::GetPrimitive(SizeT const PrimitiveIndex)
    {
        if (PrimitiveIndex >= GetNumPrimitives())
        {
            CORVUS_NO_ENTRY_FMT("Invalid StaticMeshPrimitive Index!");
        }
        return m_Primitives[PrimitiveIndex];
    }

    FTransform &CStaticMesh::GetTransform()
    {
        return m_Transform;
    }

    void CStaticMesh::SetTransform(FTransform const &Transform)
    {
        m_Transform = Transform;
    }

} // namespace Corvus
