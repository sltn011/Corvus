#ifndef CORVUS_SOURCE_CORVUS_UTILS_FILESYSTEM_H
#define CORVUS_SOURCE_CORVUS_UTILS_FILESYSTEM_H

#include "Corvus/Core/Base.h"

namespace Corvus
{

    class FFileSystem
    {
    public:
        static bool FileExists(CString const &FilePath);
        static bool DirectoryExists(CString const &FolderPath);

        static void CreateNewDirectory(CString const &DirectoryPath);

        static CString GetFileExtension(CString const &FilePath);
        static bool    IsFileExtensionEqual(CString const &FilePath, CString const &Extension);

        static CString GetCurrentPath();
        static void    SetCurrentPath(CString const &Path);
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_UTILS_FILESYSTEM_H
