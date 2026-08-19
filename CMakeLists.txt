cmake_minimum_required(VERSION 3.25...3.31)
project("TestEngine" LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5 CACHE STRING "Minimum policy version")

# Enable Hot Reload for MSVC compilers if supported
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

if (POLICY CMP0091)
  cmake_policy(SET CMP0091 NEW)
endif()

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_PREFIX_PATH "C:/Qt/6.11.0/msvc2022_64")
set(QWINDOWKIT_BUILD_WIDGETS ON CACHE BOOL "" FORCE)

include(FetchContent)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build static libraries" FORCE)
set(BUILD_QSCI_LEXERS ON CACHE BOOL "" FORCE)
set(ENABLE_LEXERS ON CACHE BOOL "" FORCE)
set(BUILD_LEXERS ON CACHE BOOL "" FORCE)

FetchContent_Declare(
    qwindowkit
    GIT_REPOSITORY https://github.com/stdware/qwindowkit.git
    GIT_TAG        main
    GIT_SUBMODULES_RECURSE TRUE
)

FetchContent_Declare(
    qscintilla
    GIT_REPOSITORY      https://github.com/farleyrunkel/QScintilla.git
    GIT_TAG             main
)

FetchContent_Declare(
    yaml-cpp
    GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
    GIT_TAG        0.8.0
)

FetchContent_Declare(
    wasmtime
    URL "https://github.com/bytecodealliance/wasmtime/releases/download/v47.0.3/wasmtime-v47.0.3-x86_64-windows-c-api.zip"
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_MakeAvailable(wasmtime)

FetchContent_MakeAvailable(qscintilla qwindowkit yaml-cpp wasmtime)

find_package(Qt6 REQUIRED COMPONENTS Widgets OpenGLWidgets PrintSupport Network)

if(WIN32 AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(QSCI_TARGET_NAME qscintilla2_qt6d)
else()
    set(QSCI_TARGET_NAME qscintilla2_qt6)
endif()

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/assets/util/ProgrammingLanguages.yml"
    "${CMAKE_CURRENT_BINARY_DIR}/assets/util/ProgrammingLanguages.yml"
    COPYONLY
)

# Add source files
add_executable(TestEngine
    "src/main.cpp"
    "include/TestEngine.h"
    "src/Environment.cpp"
    "include/Environment.h"
    "src/style.qss"
    "src/project/styles.qss"
    "src/resources.qrc"
    "src/Window.cpp"
    "include/Window.h"
    "src/project/Project.cpp"
    "include/project/Project.h"
    "include/project/EditorWindow.h"
    "src/project/EditorWindow.cpp"
    "src/project/CodeEditor.cpp"
    "include/project/CodeEditor.h"
    "src/core/Logger.cpp"
    "include/core/Logger.h"
    "src/util/Languages.cpp"
    "include/util/Languages.h"
    "include/ui/ProjectLoadingDialog.h"
    "src/core/script/WebAssembly.cpp"
    "src/core/settings/studio/StudioSettings.cpp"
    "src/core/settings/game/GameSettings.cpp"
    "src/core/settings/settings.qss"
    "include/core/settings/studio/StudioSettings.h"
    "include/core/settings/game/GameSettings.h"
    "include/util/Asset.h"
    "src/util/Asset.cpp" 
    "src/ui/docks/Explorer.cpp"
    "include/ui/docks/Explorer.h"
    "src/ui/docks/Properties.cpp"
    "include/ui/docks/Properties.h"
    "include/ui/docks/Output.h"
    "src/ui/docks/Output.cpp"
    "src/core/script/external/OpenEditor.cpp"
    "src/ui/menus/FileMenu.cpp"
    "include/ui/menus/FileMenu.h"
    "src/ui/menus/PluginsMenu.cpp"
    "include/ui/menus/PluginsMenu.h"
    "src/ui/menus/HelpMenu.cpp"
    "include/ui/menus/HelpMenu.h"
    "src/ui/menus/ViewMenu.cpp"
    "include/ui/menus/ViewMenu.h"
    "src/ui/menus/EditMenu.cpp"
    "include/ui/menus/EditMenu.h"
    "src/ui/menus/TestMenu.cpp"
    "include/ui/menus/TestMenu.h"
    "src/ui/menus/WindowMenu.cpp"
    "include/ui/menus/WindowMenu.h"
    "src/core/settings/game/tabs/GeneralTab.cpp"
    "include/core/settings/game/tabs/GeneralTab.h"
    "src/core/settings/game/tabs/AnticheatTab.cpp"
    "include/core/settings/game/tabs/AnticheatTab.h"
    "src/core/settings/game/tabs/TechnicalTab.cpp"
    "include/core/settings/game/tabs/TechnicalTab.h"
    "src/project/saving/SaveProject.cpp"
    "include/project/saving/SaveProject.h"
    "src/ui/docks/Toolbox.cpp"
    "include/ui/docks/Toolbox.h"
)

target_compile_features(TestEngine PUBLIC cxx_std_20)

target_compile_definitions(TestEngine PRIVATE
    -DUNICODE
    -D_UNICODE
    LANGUAGES_YML_PATH="${CMAKE_CURRENT_BINARY_DIR}/assets/util/ProgrammingLanguages.yml"
)

target_include_directories(TestEngine PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}/include" 
    "${CMAKE_CURRENT_SOURCE_DIR}/external/velopack_libc_1.2.0/include"
    "${qscintilla_SOURCE_DIR}/src"
    "${qscintilla_SOURCE_DIR}/scintilla/include"
    "${qscintilla_SOURCE_DIR}/scintilla/src"
    "${wasmtime_SOURCE_DIR}/include"
)

target_link_libraries(TestEngine PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}/external/velopack_libc_1.2.0/lib-static/velopack_libc_win_x64_msvc.lib"
    "${wasmtime_SOURCE_DIR}/lib/wasmtime.dll.lib"
    ws2_32
    bcrypt
    ntdll
    Qt6::Widgets
    Qt6::Network
    Qt6::OpenGLWidgets
    Qt6::PrintSupport
    QWindowKit::Widgets
    ${QSCI_TARGET_NAME}
    yaml-cpp::yaml-cpp
)

if(MSVC)
    set_target_properties(TestEngine PROPERTIES 
        LINK_FLAGS "/ENTRY:mainCRTStartup"
    )
endif()

if(WIN32)
    add_custom_command(TARGET TestEngine POST_BUILD

        # 1. Copy QWindowKit Core DLL
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_FILE:QWindowKit::Core>"
            "$<TARGET_FILE_DIR:TestEngine>"
        
        # 2. Copy QWindowKit Widgets DLL
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_FILE:QWindowKit::Widgets>"
            "$<TARGET_FILE_DIR:TestEngine>"

        # 3. Copy QScintilla DLL automatically if built dynamically
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_FILE:${QSCI_TARGET_NAME}>"
            "$<TARGET_FILE_DIR:TestEngine>"
            
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${wasmtime_SOURCE_DIR}/lib/wasmtime.dll"
            "$<TARGET_FILE_DIR:TestEngine>"

        # 4. Deploy Qt runtime components
        COMMAND "C:/Qt/6.11.0/msvc2022_64/bin/windeployqt.exe" --no-compiler-runtime "$<TARGET_FILE:TestEngine>"
        
        WORKING_DIRECTORY "$<TARGET_FILE_DIR:TestEngine>"
        COMMENT "Copying JVM toolchain, DLLs, and running windeployqt"
    )
endif()