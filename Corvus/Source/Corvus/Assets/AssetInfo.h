#ifndef CORVUS_SOURCE_CORVUS_ASSETS_ASSETINFO_H
#define CORVUS_SOURCE_CORVUS_ASSETS_ASSETINFO_H

#include "Corvus/Assets/AssetType.h"
#include "Corvus/Core/UUID.h"

#include <vector>

namespace Corvus
{

    enum class EAssetSourceType : UInt8
    {
        UNDEFINED,

        Generated,  // Asset was generated at runtime
        FromFile,   // Asset was loaded from file
        ChildAsset, // Asset is part of another Asset

        MAX_VALUE
    };

    struct SAssetInfo
    {
        EAssetType         Type;
        FUUID              UUID;
        EAssetSourceType   SourceType;
        CString            Source;
        std::vector<FUUID> References;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_ASSETS_ASSETINFO_H
