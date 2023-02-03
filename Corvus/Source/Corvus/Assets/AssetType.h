#ifndef CORVUS_SOURCE_CORVUS_ASSETS_ASSETTYPE_H
#define CORVUS_SOURCE_CORVUS_ASSETS_ASSETTYPE_H

#include "Corvus/Core/CoreTypes.h"

namespace Corvus
{

    enum class EAssetType : UInt16
    {
        Texture2D,
        Material,
        StaticModel,

        MAX_VALUE
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_ASSETS_ASSETTYPE_H
