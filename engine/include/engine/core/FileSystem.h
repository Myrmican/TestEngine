#pragma once

#include <string>

namespace Engine {
    // Returns the directory the running executable lives in, with a
    // trailing slash - e.g. "C:/MyProject/build/Release/"
    std::wstring getExecutableDirectory();
}