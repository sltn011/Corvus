#include "CorvusPCH.h"

#include "Corvus/Utils/FileSystem.h"

#include <filesystem>

namespace Corvus
{
    bool FFileSystem::FileExists(CString const &FilePath)
    {
        return std::filesystem::exists(FilePath);
    }

    bool FFileSystem::DirectoryExists(CString const &DirectoryPath)
    {
        return std::filesystem::exists(DirectoryPath);
    }

    void FFileSystem::CreateNewDirectory(CString const &DirectoryPath)
    {
        std::filesystem::create_directories(DirectoryPath);
    }

    CString FFileSystem::CombinePaths(CString const &Lhs, CString const &Rhs)
    {
        std::filesystem::path LhsPath{Lhs};
        std::filesystem::path RhsPath{Rhs};
        return std::filesystem::path{LhsPath / RhsPath}.string();
    }

    CString FFileSystem::GetFileName(CString const &FilePath)
    {
        CORVUS_CORE_ASSERT(FileExists(FilePath));

        return std::filesystem::path{FilePath}.stem().string();
    }

    CString FFileSystem::GetFileExtension(CString const &FilePath)
    {
        CORVUS_CORE_ASSERT(FileExists(FilePath));

        return std::filesystem::path{FilePath}.extension().string();
    }

    bool FFileSystem::IsFileExtensionEqual(CString const &FilePath, CString const &Extension)
    {
        return GetFileExtension(FilePath) == Extension;
    }

    CString FFileSystem::GetCurrentPath()
    {
        return std::filesystem::canonical(std::filesystem::current_path()).string();
    }

    void FFileSystem::SetCurrentPath(CString const &Path)
    {
        CORVUS_CORE_ASSERT(DirectoryExists(Path));
        std::filesystem::current_path(Path);
    }

    CString FFileSystem::GetParentPath()
    {
        return std::filesystem::current_path().parent_path().string();
    }

    CString::value_type FFileSystem::GetPathSeparator()
    {
        if (std::filesystem::path::preferred_separator == L'\\')
        {
            return '\\';
        }
        return '/';
    }

} // namespace Corvus