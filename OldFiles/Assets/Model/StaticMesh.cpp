#include "CorvusPCH.h"

#include "Corvus/Assets/Model/StaticMesh.h"

namespace Corvus
{

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

} // namespace Corvus
