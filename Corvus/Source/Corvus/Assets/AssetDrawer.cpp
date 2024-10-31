#include "CorvusPCH.h"

#include "AssetDrawer.h"

#include "Corvus/Assets/Material/Material.h"
#include "Corvus/Assets/Model/ModelLoader.h"
#include "Corvus/Assets/Model/StaticModel.h"
#include "Corvus/Assets/Texture/Texture2D.h"
#include "Corvus/Renderer/Renderer.h"

namespace Corvus
{

    FUUID Corvus::CAssetDrawer::LoadStaticModelFromFile(CString const &FilePath)
    {
        SStaticModelLoadedData LoadedData = CModelLoader::LoadStaticModelFromFile(FilePath);

        // Copy loaded data to AssetDrawer
        for (CTexture2D &Texture : LoadedData.Textures)
        {
            Textures2D[Texture.UUID] = Texture;
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
            Material.Albedo  = Textures2D.at(AlbedoUUID);

            FUUID NormalUUID = Material.Normal.UUID;
            Material.Normal  = Textures2D.at(NormalUUID);

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
        for (auto &[UUID, Texture2D] : Textures2D)
        {
            Renderer().DestroyTexture2D(Texture2D);
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
