#ifndef CORVUSEDITOR_SOURCE_CORVUSEDITOR_SOURCE_GUI_PANELS_ASSETSPANEL_H
#define CORVUSEDITOR_SOURCE_CORVUSEDITOR_SOURCE_GUI_PANELS_ASSETSPANEL_H

#include "Corvus/Utils/FileSystem.h"
#include "CorvusEditor/GUI/Panels/Panel.h"

#include <filesystem>

namespace Corvus
{

    class CAssetsPanel : public CPanel
    {
    public:
        using Super = CPanel;

        CAssetsPanel(
            CString AssetsDirectoryPath  = "Assets",
            CString ApplicationDirectory = FFileSystem::GetCurrentPath()
        );

        virtual void Render(FTimeDelta ElapsedTime, EPanelFlags PanelFlags) override;

    private:
        void RenderControlButtons();
        void RenderAssetsTable();
        void RenderEntry(std::filesystem::directory_entry const &Entry);
        void RenderDirectoryEntry(std::filesystem::directory_entry const &Directory);
        void RenderFileEntry(std::filesystem::directory_entry const &File);
        void RenderParentDirectory();

    private:
        CString m_AssetsDirectoryPath;
        CString m_ApplicationDirectory;

        CString m_CurrentSelectedDirectory;
        Int32   m_CurrentDepth = 0;
    };

} // namespace Corvus

#endif // !CORVUSEDITOR_SOURCE_CORVUSEDITOR_SOURCE_GUI_PANELS_ASSETSPANEL_H