#include "CorvusPCH.h"

#include "Corvus/Assets/Model/ModelLoader.h"

#include "Corvus/Assets/Model/GLTFModelLoader.h"
#include "Corvus/Assets/Model/StaticModel.h"

namespace Corvus
{

    CStaticModel CModelLoader::LoadStaticModelFromFile(CString const &FilePath)
    {
        CORVUS_CORE_TRACE("Loading StaticMesh {}", FilePath);
        FTimePoint ModelLoadStart;

        SizeT FormatPrefixIndex = FilePath.find_last_of('.');
        if (FormatPrefixIndex == CString::npos)
        {
            CORVUS_CORE_ERROR("Invalid file path passed to ModelLoader: {}", FilePath);
            return CStaticModel();
        }

        CString const Format = FilePath.substr(FormatPrefixIndex + 1);

        CStaticModel StaticModel;
        if (Format == "gltf") // glTF
        {
            StaticModel = CGLTFModelLoader::LoadStaticModelFromFile(FilePath, false);
        }
        else if (Format == "glb") // Binary glTF
        {
            StaticModel = CGLTFModelLoader::LoadStaticModelFromFile(FilePath, true);
        }
        else
        {
            CORVUS_CORE_ERROR("Unknown file format \"{}\" passed to ModelLoader: {}", Format, FilePath);
            return StaticModel;
        }

        FTimePoint ModelLoadEnd;
        FTimeDelta ModelLoadTime = ModelLoadEnd - ModelLoadStart;
        CORVUS_CORE_TRACE("Loaded Model {0} in {1}ms", FilePath, ModelLoadTime.MilliSeconds());

        return StaticModel;
    }

} // namespace Corvus
