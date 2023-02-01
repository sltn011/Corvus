#ifndef CORVUSEDITOR_SOURCE_CORVUSEDITOR_SOURCE_GUI_PANELS_ASSETSPANEL_H
#define CORVUSEDITOR_SOURCE_CORVUSEDITOR_SOURCE_GUI_PANELS_ASSETSPANEL_H

#include "CorvusEditor/GUI/Panels/Panel.h"

#include <filesystem>

namespace Corvus
{

    class CAssetsPanel : public CPanel
    {
    public:
        using Super = CPanel;

        CAssetsPanel(
            std::filesystem::path AssetsDirectoryPath  = "Assets",
            std::filesystem::path ApplicationDirectory = std::filesystem::current_path()
        );

        virtual void Render(FTimeDelta ElapsedTime, EPanelFlags PanelFlags) override;

    private:
        void RenderEntry(std::filesystem::directory_entry const &Entry);
        void RenderFolderEntry(std::filesystem::directory_entry const &Folder);
        void RenderFileEntry(std::filesystem::directory_entry const &File);

    private:
        std::filesystem::path m_AssetsDirectoryPath;
        std::filesystem::path m_ApplicationDirectory;
    };

} // namespace Corvus

#endif // !CORVUSEDITOR_SOURCE_CORVUSEDITOR_SOURCE_GUI_PANELS_ASSETSPANEL_H