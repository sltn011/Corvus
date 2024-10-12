#ifndef CORVUS_SOURCE_CORVUS_ASSETS_ASSETDRAWER_H
#define CORVUS_SOURCE_CORVUS_ASSETS_ASSETDRAWER_H

#include "Corvus/Core/UUID.h"

#include <unordered_map>

namespace Corvus
{
    class CTexture2D;
    class CMaterial;
    class CStaticModel;

    class CAssetDrawer
    {
    public:
        std::unordered_map<FUUID, CTexture2D>   TexturesAssets;
        std::unordered_map<FUUID, CMaterial>    MaterialsAssets;
        std::unordered_map<FUUID, CStaticModel> StaticModelsAssets;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_ASSETS_ASSETDRAWER_H
