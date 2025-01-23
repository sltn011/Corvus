#ifndef CORVUS_SOURCE_CORVUS_ASSETS_ASSETDRAWER_H
#define CORVUS_SOURCE_CORVUS_ASSETS_ASSETDRAWER_H

#include "Corvus/Assets/Default/DefaultTextures.h"
#include "Corvus/Core/UUID.h"

#include <unordered_map>

namespace Corvus
{

    class CTexture;
    class CMaterial;
    class CStaticModel;

    class CAssetDrawer
    {
    public:
        FUUID LoadStaticModelFromFile(CString const &FilePath);

        void DestroyRenderData();

    public:
        std::unordered_map<FUUID, CTexture>     Textures;
        std::unordered_map<FUUID, CMaterial>    Materials;
        std::unordered_map<FUUID, CStaticModel> StaticModels;

    public:
        CDefaultTextures DefaultTextures;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_ASSETS_ASSETDRAWER_H
