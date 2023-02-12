#ifndef CORVUS_SOURCE_CORVUS_ASSETS_SERIALIZER_SERIALIZER_H
#define CORVUS_SOURCE_CORVUS_ASSETS_SERIALIZER_SERIALIZER_H

#include "Corvus/Assets/Asset.h"
#include "Corvus/Assets/Model/StaticMeshPrimitive.h"
#include "Corvus/Math/Transform.h"

#include <type_traits>

namespace Corvus
{

    class CAssetSerializer
    {
    public:
        template<typename TAssetType, std::enable_if_t<std::is_base_of_v<CAsset, TAssetType>, bool> = true>
        static void Serialize(TAssetType const &Asset, CString const &AssetDirectoryPath = "")
        {
            SerializeImpl(Asset.GetAssetInfo(), AssetDirectoryPath);
        }

    private:
        static void SerializeImpl(SAssetInfo const &AssetInfo, CString const &AssetFilePath);
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_ASSETS_MODEL_STATICMESH_H
