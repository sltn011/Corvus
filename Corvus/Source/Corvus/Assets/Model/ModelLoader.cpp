#include "CorvusPCH.h"

#include "Corvus/Assets/Model/ModelLoader.h"

#include "Corvus/Assets/Model/GLTFModelLoader.h"
#include "Corvus/Assets/Model/StaticModel.h"
#include "Corvus/Assets/Texture/Texture2D.h"
#include "Corvus/Renderer/Shader.h"
#include "Corvus/Renderer/Texture2DBuffer.h"

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

        FTimePoint ModelLoadEnd;
        FTimeDelta ModelLoadTime = ModelLoadEnd - ModelLoadStart;
        CORVUS_CORE_TRACE("Loaded Model {0} in {1}ms", FilePath, ModelLoadTime.MilliSeconds());

        return LoadedData;
    }

} // namespace Corvus
