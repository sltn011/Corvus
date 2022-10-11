#include "CorvusPCH.h"

#include "Corvus/Assets/Model/StaticModel.h"

namespace Corvus
{
    SizeT CStaticModel::AddMesh(CStaticMesh &&Mesh)
    {
        SizeT MeshIndex = GetNumMeshes();
        m_Meshes.emplace_back(std::move(Mesh));
        return MeshIndex;
    }

    SizeT CStaticModel::GetNumMeshes() const
    {
        return m_Meshes.size();
    }

    CStaticMesh &CStaticModel::GetMesh(SizeT MeshIndex)
    {
        if (MeshIndex >= GetNumMeshes())
        {
            CORVUS_NO_ENTRY_FMT("Invalid StaticMesh Index!");
        }
        return m_Meshes[MeshIndex];
    }

} // namespace Corvus
