#ifndef CORVUS_SOURCE_CORVUS_ASSETS_MODEL_GLTFMODELLOADER_H
#define CORVUS_SOURCE_CORVUS_ASSETS_MODEL_GLTFMODELLOADER_H

#include "Corvus/Assets/Model/ModelLoader.h"

namespace Corvus
{

    class CStaticModel;

    class CGLTFModelLoader
    {
    public:
        static SStaticModelLoadedData LoadStaticModelFromFile(CString const &FilePath, bool bIsBinary);
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_ASSETS_MODEL_GLTFMODELLOADER_H
