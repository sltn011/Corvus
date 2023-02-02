#include "CorvusEditorPCH.h"

#include "CorvusEditor/GUI/Panels/AssetsPanel.h"

#include <filesystem>

namespace Corvus
{

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

        m_CurrentSelectedDirectory = m_AssetsDirectoryPath;
    }

    void CAssetsPanel::Render(FTimeDelta ElapsedTime, EPanelFlags PanelFlags)
    {
        FFileSystem::SetCurrentPath(m_CurrentSelectedDirectory);

        if (ImGui::Begin("Assets", nullptr, EnumRawValue(PanelFlags)))
        {
            ImGui::Text(
                "Current Directory: %c%s", FFileSystem::GetPathSeparator(), m_CurrentSelectedDirectory.c_str()
            );

            ImGui::NewLine();

            static constexpr EEditorTableFlags AssetsDirectoryTreeFlags =
                EEditorTableFlags::Resizable | EEditorTableFlags::RowBg | EEditorTableFlags::NoBordersInBody;

            if (ImGui::BeginTable("AssetsDirectoryTree", 2, EnumRawValue(AssetsDirectoryTreeFlags)))
            {
                ImGui::TableSetupColumn("Name");
                ImGui::TableSetupColumn("Type");
                ImGui::TableHeadersRow();

                if (m_CurrentDepth != 0)
                {
                    RenderParentDirectory();
                }

                std::filesystem::directory_iterator DirectoryIterator{FFileSystem::GetCurrentPath()};
                for (std::filesystem::directory_entry const &SubEntry : DirectoryIterator)
                {
                    RenderEntry(SubEntry);
                }

                ImGui::EndTable();
            }
        }
        ImGui::End();

        FFileSystem::SetCurrentPath(m_ApplicationDirectory);
    }

    void CAssetsPanel::RenderEntry(std::filesystem::directory_entry const &Entry)
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

    void CAssetsPanel::RenderDirectoryEntry(std::filesystem::directory_entry const &Directory)
    {
        ImGui::TableNextRow();
        ImGui::TableNextColumn();

        std::filesystem::path DirectoryPath = Directory.path();

        CString PathStr   = DirectoryPath.string();
        SizeT   Delimiter = PathStr.find_last_of(FFileSystem::GetPathSeparator());
        CORVUS_CORE_ASSERT(Delimiter != CString::npos);

        CString FolderName = PathStr.substr(Delimiter);
        ImGui::Selectable(FolderName.c_str());
        bool bSelected = ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0);
        ImGui::TableNextColumn();
        ImGui::Text("Directory");
        if (bSelected)
        {
            m_CurrentSelectedDirectory += FolderName;
            m_CurrentDepth++;
        }
    }

    void CAssetsPanel::RenderFileEntry(std::filesystem::directory_entry const &File)
    {
        ImGui::TableNextRow();
        ImGui::TableNextColumn();

        std::filesystem::path FilePath = File.path();

        ImGui::Text(FilePath.filename().string().c_str());
        ImGui::TableNextColumn();
        ImGui::Text("File");
    }

    void CAssetsPanel::RenderParentDirectory()
    {
        ImGui::TableNextRow();
        ImGui::TableNextColumn();

        CString ParentShortName = CString{FFileSystem::GetPathSeparator()} + CString{".."};

        ImGui::Selectable(ParentShortName.c_str());
        bool bSelected = ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0);
        ImGui::TableNextColumn();
        ImGui::Text("Directory");
        if (bSelected)
        {
            SizeT LastDelimiter = m_CurrentSelectedDirectory.find_last_of(FFileSystem::GetPathSeparator());
            CORVUS_CORE_ASSERT(LastDelimiter != CString::npos);

            m_CurrentSelectedDirectory = m_CurrentSelectedDirectory.substr(0, LastDelimiter);
            m_CurrentDepth--;
            CORVUS_CORE_ASSERT(m_CurrentDepth >= 0);
        }
    }

} // namespace Corvus
