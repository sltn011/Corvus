#include "CorvusPCH.h"

#include "AssetDrawer.h"

#include "Corvus/Assets/Material/Material.h"
#include "Corvus/Assets/Model/ModelLoader.h"
#include "Corvus/Assets/Model/StaticModel.h"
#include "Corvus/Assets/Texture/Texture.h"
#include "Corvus/Renderer/Renderer.h"

namespace Corvus
{

    FUUID Corvus::CAssetDrawer::LoadStaticModelFromFile(CString const &FilePath)
    {
        SStaticModelLoadedData LoadedData = CModelLoader::LoadStaticModelFromFile(FilePath);

        // Copy loaded data to AssetDrawer
        for (CTexture &Texture : LoadedData.Textures)
        {
            Textures[Texture.UUID] = Texture;
        }
        for (CMaterial &Material : LoadedData.Materials)
        {
            Materials[Material.UUID] = Material;
        }
        StaticModels[LoadedData.StaticModel.UUID] = LoadedData.StaticModel;

        // Configure materials
        for (CMaterial const &LoadedMaterial : LoadedData.Materials)
        {
            CMaterial &Material = Materials[LoadedMaterial.UUID]; // must use Material stored in AssetDrawer

            FUUID AlbedoUUID = Material.Albedo.UUID;
            Material.Albedo  = Textures.at(AlbedoUUID);

            FUUID NormalUUID = Material.Normal.UUID;
            Material.Normal  = Textures.at(NormalUUID);

            Renderer().CreateMaterialRenderData(Material); // Configure Material render data
        }

        // Configure StaticMeshPrimitives
        CStaticModel &StaticModel =
            StaticModels[LoadedData.StaticModel.UUID]; // must use StaticModel stored in AssetDrawer
        for (CStaticMesh &Mesh : StaticModel)
        {
            for (CStaticMeshPrimitive &Primitive : Mesh)
            {
                FUUID MaterialUUID = Primitive.Material.UUID;
                Primitive.Material = Materials[MaterialUUID];
            }
        }

        return LoadedData.StaticModel.UUID;
    }

    void CAssetDrawer::DestroyRenderData()
    {
        for (auto &[UUID, Texture] : Textures)
        {
            Renderer().DestroyTexture(Texture);
        }
        for (auto &[UUID, Material] : Materials)
        {
            Renderer().DestroyMaterialRenderData(Material);
        }
        for (auto &[UUID, StaticModel] : StaticModels)
        {
            for (CStaticMesh &Mesh : StaticModel)
            {
                for (CStaticMeshPrimitive &Primitive : Mesh)
                {
                    Renderer().DestroyBuffer(Primitive.VertexBuffer);
                    Renderer().DestroyBuffer(Primitive.IndexBuffer);
                }
            }
        }
    }

} // namespace Corvus
