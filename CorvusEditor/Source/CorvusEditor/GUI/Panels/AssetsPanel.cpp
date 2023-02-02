#include "CorvusEditorPCH.h"

#include "CorvusEditor/GUI/Panels/AssetsPanel.h"

#include <filesystem>

namespace Corvus
{
    void RenderEntry(std::filesystem::directory_entry const &Entry);
    void RenderDirectoryEntry(std::filesystem::directory_entry const &Directory);
    void RenderFileEntry(std::filesystem::directory_entry const &File);

    CAssetsPanel::CAssetsPanel(CString AssetsDirectoryPath, CString ApplicationDirectory)
        : m_AssetsDirectoryPath{std::move(AssetsDirectoryPath)},
          m_ApplicationDirectory{std::move(ApplicationDirectory)}
    {
        if (!FFileSystem::DirectoryExists(m_AssetsDirectoryPath))
        {
            FFileSystem::CreateNewDirectory(m_AssetsDirectoryPath);
        }

        if (!FFileSystem::DirectoryExists(m_ApplicationDirectory))
        {
            CORVUS_CORE_NO_ENTRY_FMT(
                "Specified Application Directory {} does not exist!", m_ApplicationDirectory
            );
        }
    }

    void CAssetsPanel::Render(FTimeDelta ElapsedTime, EPanelFlags PanelFlags)
    {
        FFileSystem::SetCurrentPath(m_AssetsDirectoryPath);

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

        FFileSystem::SetCurrentPath(m_ApplicationDirectory);
    }

    void RenderEntry(std::filesystem::directory_entry const &Entry)
    {
        if (Entry.is_directory())
        {
            RenderDirectoryEntry(Entry);
            return;
        }
        if (Entry.is_regular_file())
        {
            RenderFileEntry(Entry);
            return;
        }
    }

    void RenderDirectoryEntry(std::filesystem::directory_entry const &Directory)
    {
        ImGui::TableNextRow();
        ImGui::TableNextColumn();

        std::filesystem::path DirectoryPath = Directory.path();

        CString PathStr   = DirectoryPath.string();
        SizeT   Delimiter = PathStr.find_last_of(std::filesystem::path::preferred_separator);
        if (Delimiter != CString::npos)
        {
            bool bOpen = ImGui::TreeNode(PathStr.substr(Delimiter + 1).c_str());
            ImGui::TableNextColumn();
            ImGui::Text("Directory");
            if (bOpen)
            {
                std::filesystem::directory_iterator DirectoryIterator{DirectoryPath};
                for (std::filesystem::directory_entry const &SubEntry : DirectoryIterator)
                {
                    RenderEntry(SubEntry);
                }
                ImGui::TreePop();
            }
        }
        else
        {
            CORVUS_CORE_WARN("Unknown directory found while rendering Assets tree!");
        }
    }

    void RenderFileEntry(std::filesystem::directory_entry const &File)
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
