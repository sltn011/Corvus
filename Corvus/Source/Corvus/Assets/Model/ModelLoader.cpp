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

        SizeT FormatPrefixIndex = FilePath.find_last_of('.');
        if (FormatPrefixIndex == CString::npos)
        {
            CORVUS_CORE_ERROR("Invalid file path passed to ModelLoader: {}", FilePath);
            return SStaticModelLoadedData();
        }

        CString const Format = FilePath.substr(FormatPrefixIndex + 1);

        SStaticModelLoadedData LoadedData;
        if (Format == "gltf") // glTF
        {
            LoadedData = CGLTFModelLoader::LoadStaticModelFromFile(FilePath, false);
        }
        else if (Format == "glb") // Binary glTF
        {
            LoadedData = CGLTFModelLoader::LoadStaticModelFromFile(FilePath, true);
        }
        else
        {
            CORVUS_CORE_ERROR("Unknown file format \"{}\" passed to ModelLoader: {}", Format, FilePath);
            return LoadedData;
        }

        FTimePoint ModelLoadEnd;
        FTimeDelta ModelLoadTime = ModelLoadEnd - ModelLoadStart;
        CORVUS_CORE_TRACE("Loaded Model {0} in {1}ms", FilePath, ModelLoadTime.MilliSeconds());

        return LoadedData;
    }

} // namespace Corvus
