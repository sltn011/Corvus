#ifndef CORVUS_SOURCE_CORVUS_ASSETS_ASSET_H
#define CORVUS_SOURCE_CORVUS_ASSETS_ASSET_H

#include "Corvus/Assets/AssetInfo.h"
#include "Corvus/Assets/AssetType.h"
#include "Corvus/Core/UUID.h"

namespace Corvus
{

    class CAsset
    {
    public:
        CAsset(EAssetType Type) : AssetType{Type} {}
        virtual ~CAsset() = default;

        virtual SAssetInfo GetAssetInfo() const = 0;

        FUUID      UUID;
        EAssetType AssetType;

        CString AssetName;

        EAssetSourceType SourceType = EAssetSourceType::UNDEFINED;
        CString          Source;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_ASSETS_ASSETINFO_H
