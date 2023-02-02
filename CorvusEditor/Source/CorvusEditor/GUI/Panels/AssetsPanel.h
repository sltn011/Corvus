#ifndef CORVUSEDITOR_SOURCE_CORVUSEDITOR_SOURCE_GUI_PANELS_ASSETSPANEL_H
#define CORVUSEDITOR_SOURCE_CORVUSEDITOR_SOURCE_GUI_PANELS_ASSETSPANEL_H

#include "Corvus/Utils/FileSystem.h"
#include "CorvusEditor/GUI/Panels/Panel.h"

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
        CString m_AssetsDirectoryPath;
        CString m_ApplicationDirectory;
    };

} // namespace Corvus

#endif // !CORVUSEDITOR_SOURCE_CORVUSEDITOR_SOURCE_GUI_PANELS_ASSETSPANEL_H