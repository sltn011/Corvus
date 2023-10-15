#ifndef CORVUS_SOURCE_CORVUS_ASSETS_MODEL_MODELLOADER_H
#define CORVUS_SOURCE_CORVUS_ASSETS_MODEL_MODELLOADER_H

#include "Corvus/Assets/Model/StaticModel.h"
#include "Corvus/Core/Base.h"

namespace Corvus
{

    class CMaterial;
    class CStaticModel;
    struct CTexture2D;

    struct SStaticModelLoadedData
    {
        CStaticModel            StaticModel;
        std::vector<CTexture2D> Textures;
        std::vector<CMaterial>  Materials;
    };

    class CModelLoader
    {
    public:
        static [[nodiscard]] SStaticModelLoadedData LoadStaticModelFromFile(CString const &FilePath);
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_ASSETS_MODEL_MODELLOADER_H
