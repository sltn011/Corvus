#include "CorvusPCH.h"

#include "Corvus/Assets/Model/ModelLoader.h"

#include "Corvus/Assets/Material/Material.h"
#include "Corvus/Assets/Model/GLTFModelLoader.h"
#include "Corvus/Assets/Model/StaticModel.h"
// #include "Corvus/Assets/Texture/Texture2D.h"

namespace Corvus
{

    SStaticModelLoadedData CModelLoader::LoadStaticModelFromFile(CString const &FilePath)
    {
        CORVUS_CORE_TRACE("Loading StaticMesh {}", FilePath);
        FTimePoint ModelLoadStart;

        if (!FFileSystem::FileExists(FilePath))
        {
            CORVUS_CORE_ERROR("StaticModel file {} not found!", FilePath);
            return {};
        }

        SStaticModelLoadedData LoadedData;
        if (FFileSystem::IsFileExtensionEqual(FilePath, ".gltf")) // glTF
        {
            LoadedData = CGLTFModelLoader::LoadStaticModelFromFile(FilePath, false);
        }
        else if (FFileSystem::IsFileExtensionEqual(FilePath, ".glb")) // Binary glTF
        {
            LoadedData = CGLTFModelLoader::LoadStaticModelFromFile(FilePath, true);
        }
        else
        {
            CORVUS_CORE_ERROR(
                "Unknown file format \"{}\" passed to ModelLoader: {}",
                FFileSystem::GetFileExtension(FilePath),
                FilePath
            );
            return LoadedData;
        }

        LoadedData.StaticModel.AssetName  = FFileSystem::GetFileName(FilePath);
        LoadedData.StaticModel.SourceType = EAssetSourceType::FromFile;
        LoadedData.StaticModel.Source     = FilePath;

        for (CTexture2D &Texture : LoadedData.Textures)
        {
            Texture.SourceType = EAssetSourceType::ChildAsset;
            Texture.Source     = LoadedData.StaticModel.UUID.ToString();
        }

        for (CMaterial &Material : LoadedData.Materials)
        {
            Material.SourceType = EAssetSourceType::ChildAsset;
            Material.Source     = LoadedData.StaticModel.UUID.ToString();
        }

        FTimePoint ModelLoadEnd;
        FTimeDelta ModelLoadTime = ModelLoadEnd - ModelLoadStart;
        CORVUS_CORE_TRACE("Loaded Model {0} in {1}ms", FilePath, ModelLoadTime.MilliSeconds());

        return LoadedData;
    }

} // namespace Corvus
