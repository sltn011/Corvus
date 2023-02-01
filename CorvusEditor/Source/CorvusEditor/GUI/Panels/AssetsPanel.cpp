#include "CorvusEditorPCH.h"

#include "CorvusEditor/GUI/Panels/AssetsPanel.h"

namespace Corvus
{

    CAssetsPanel::CAssetsPanel(
        std::filesystem::path AssetsDirectoryPath, std::filesystem::path ApplicationDirectory
    )
        : m_AssetsDirectoryPath{std::move(AssetsDirectoryPath)},
          m_ApplicationDirectory{std::move(ApplicationDirectory)}
    {
        if (!std::filesystem::exists(m_AssetsDirectoryPath))
        {
            std::filesystem::create_directories(m_AssetsDirectoryPath);
        }

        if (!std::filesystem::exists(m_ApplicationDirectory))
        {
            CORVUS_CORE_CRITICAL("Specified Application Directory does not exist!");
        }
    }

    void CAssetsPanel::Render(FTimeDelta ElapsedTime, EPanelFlags PanelFlags)
    {
        std::filesystem::current_path(m_AssetsDirectoryPath);

        if (ImGui::Begin("Assets", nullptr, EnumRawValue(PanelFlags)))
        {
            static constexpr EEditorTableFlags AssetsDirectoryTreeFlags =
                EEditorTableFlags::Resizable | EEditorTableFlags::RowBg | EEditorTableFlags::NoBordersInBody;

            if (ImGui::BeginTable("AssetsDirectoryTree", 2, EnumRawValue(AssetsDirectoryTreeFlags)))
            {
                ImGui::TableSetupColumn("Name");
                ImGui::TableSetupColumn("Type");
                ImGui::TableHeadersRow();

                std::filesystem::path               CurrentDirectory = std::filesystem::current_path();
                std::filesystem::directory_iterator DirectoryIterator{CurrentDirectory};
                for (std::filesystem::directory_entry const &Entry : DirectoryIterator)
                {
                    RenderEntry(Entry);
                }
                ImGui::EndTable();
            }
        }
        ImGui::End();

        std::filesystem::current_path(m_ApplicationDirectory);
    }

    void CAssetsPanel::RenderEntry(std::filesystem::directory_entry const &Entry)
    {
        if (Entry.is_directory())
        {
            RenderFolderEntry(Entry);
            return;
        }
        if (Entry.is_regular_file())
        {
            RenderFileEntry(Entry);
            return;
        }
    }

    void CAssetsPanel::RenderFolderEntry(std::filesystem::directory_entry const &Folder)
    {
        ImGui::TableNextRow();
        ImGui::TableNextColumn();

        std::filesystem::path FolderPath = Folder.path();

        CString PathStr   = FolderPath.string();
        SizeT   Delimiter = PathStr.find_last_of(std::filesystem::path::preferred_separator);
        if (Delimiter != CString::npos)
        {
            bool bOpen = ImGui::TreeNode(PathStr.substr(Delimiter + 1).c_str());
            ImGui::TableNextColumn();
            ImGui::Text("Folder");
            if (bOpen)
            {
                std::filesystem::directory_iterator DirectoryIterator{FolderPath};
                for (std::filesystem::directory_entry const &SubEntry : DirectoryIterator)
                {
                    RenderEntry(SubEntry);
                }
                ImGui::TreePop();
            }
        }
        else
        {
            CORVUS_CORE_WARN("Unknown folder found while rendering Assets tree!");
        }
    }

    void CAssetsPanel::RenderFileEntry(std::filesystem::directory_entry const &File)
    {
        ImGui::TableNextRow();
        ImGui::TableNextColumn();

        std::filesystem::path FilePath = File.path();

        ImGui::Selectable(FilePath.filename().string().c_str());
        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
        {
            CORVUS_WARN("Clicked!");
        }
        ImGui::TableNextColumn();
        ImGui::Text("File");
    }

} // namespace Corvus
