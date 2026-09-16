#include "core/FileSystem.h"
#include <windows.h>

namespace Engine {

    std::wstring getExecutableDirectory()
    {
        wchar_t path[MAX_PATH];
        DWORD length = GetModuleFileNameW(nullptr, path, MAX_PATH);
        if (length == 0)
            return L""; // failed - caller should handle this being empty

        std::wstring fullPath(path, length);

        // Strip the filename, keep just the directory portion.
        // e.g. "C:\MyProject\build\Release\TestEngine.exe"
        //   -> "C:\MyProject\build\Release\"
        size_t lastSlash = fullPath.find_last_of(L"\\/");
        if (lastSlash != std::wstring::npos)
            return fullPath.substr(0, lastSlash + 1);

        return L"";
    }

}