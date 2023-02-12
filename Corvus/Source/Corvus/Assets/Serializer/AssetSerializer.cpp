#include "CorvusPCH.h"

#include "Corvus/Assets/Serializer/AssetSerializer.h"

#include <filesystem>
#include <iomanip>
#include <json.hpp>

namespace Corvus
{

    void CAssetSerializer::SerializeImpl(SAssetInfo const &AssetInfo, CString const &AssetDirectoryPath)
    {
        if (!FFileSystem::DirectoryExists(AssetDirectoryPath))
        {
            CORVUS_ERROR("Asset directory path {} does not exist!", AssetDirectoryPath);
            return;
        }

        nlohmann::json AssetJSON;
        AssetJSON["Type"]       = EnumRawValue(AssetInfo.Type);
        AssetJSON["UUID"]       = AssetInfo.UUID.ToString();
        AssetJSON["Name"]       = AssetInfo.Name;
        AssetJSON["SourceType"] = EnumRawValue(AssetInfo.SourceType);
        AssetJSON["Source"]     = AssetInfo.Source;

        std::vector<CString> ReferencesStringArray(AssetInfo.References.size());
        std::transform(
            AssetInfo.References.begin(),
            AssetInfo.References.end(),
            ReferencesStringArray.begin(),
            [](FUUID const &UUID)
            {
                return UUID.ToString();
            }
        );
        AssetJSON["References"] = ReferencesStringArray;

        CString AssetFileName = !AssetInfo.Name.empty() ? AssetInfo.Name : AssetInfo.UUID.ToString();
        AssetFileName += ".casset";
        CString AssetFilePath = FFileSystem::CombinePaths(AssetDirectoryPath, AssetFileName);

        std::ofstream AssetFile(AssetFilePath);
        AssetFile << std::setw(4) << AssetJSON;

        CORVUS_TRACE("Asset {} serialized to {}", AssetInfo.Name, AssetFilePath);
    }

} // namespace Corvus
