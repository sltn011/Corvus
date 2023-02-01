#include "CorvusEditorPCH.h"

#include "CorvusEditor/GUI/Panels/AssetsPanel.h"

#include <filesystem>

namespace Corvus
{
    void RenderEntry(std::filesystem::directory_entry const &Entry);
    void RenderFolderEntry(std::filesystem::directory_entry const &Folder);
    void RenderFileEntry(std::filesystem::directory_entry const &File);

    void CAssetsPanel::Render(FTimeDelta ElapsedTime, EPanelFlags PanelFlags)
    {
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
    }

    void RenderEntry(std::filesystem::directory_entry const &Entry)
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

    void RenderFolderEntry(std::filesystem::directory_entry const &Folder)
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

    void RenderFileEntry(std::filesystem::directory_entry const &File)
    {
        ImGui::TableNextRow();
        ImGui::TableNextColumn();

        std::filesystem::path FilePath = File.path();

        ImGui::Text(FilePath.filename().string().c_str());
        ImGui::TableNextColumn();
        ImGui::Text("File");
    }

} // namespace Corvus
