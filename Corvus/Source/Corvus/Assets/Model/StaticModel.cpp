#include "CorvusPCH.h"

#include "Corvus/Assets/Model/StaticModel.h"

namespace Corvus
{

    CStaticModel::CStaticModel() : Super{EAssetType::StaticModel}
    {
    }

    SAssetInfo CStaticModel::GetAssetInfo() const
    {
        SAssetInfo AssetInfo{};
        AssetInfo.Type       = AssetType;
        AssetInfo.UUID       = UUID;
        AssetInfo.Name       = AssetName;
        AssetInfo.SourceType = SourceType;
        AssetInfo.Source     = Source;

        for (CStaticMesh const &Mesh : m_Meshes)
        {
            for (CStaticMeshPrimitive const &Primitive : Mesh)
            {
                AssetInfo.References.push_back(Primitive.MaterialRef.GetUUID());
            }
        }

        return AssetInfo;
    }

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
