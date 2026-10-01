cmake_minimum_required(VERSION 3.25...3.31)

if(NOT CMAKE_PREFIX_PATH AND NOT Qt6_DIR)
    set(CMAKE_PREFIX_PATH "C:/Qt/6.11.2/msvc2022_64")
endif()

project("TestEngine" LANGUAGES CXX)

if(MSVC)
    add_compile_options(/external:W0 /external:anglebrackets)
endif()

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

set(QWINDOWKIT_BUILD_WIDGETS ON CACHE BOOL "" FORCE)
set(SDL_TESTS OFF CACHE BOOL "" FORCE)

include(FetchContent)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build static libraries" FORCE)
set(BUILD_QSCI_LEXERS ON CACHE BOOL "" FORCE)
set(ENABLE_LEXERS ON CACHE BOOL "" FORCE)
set(BUILD_LEXERS ON CACHE BOOL "" FORCE)

if (MSVC)
    # Enable PDB generation for Debug and Release builds
    add_compile_options(/Zi)
    add_link_options(/DEBUG /OPT:REF /OPT:ICF)
endif()

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
    SOURCE_SUBDIR  _no_cmake_here
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

find_package(Qt6 REQUIRED COMPONENTS Widgets OpenGLWidgets PrintSupport Network)

FetchContent_MakeAvailable(qscintilla qwindowkit yaml-cpp wasmtime)

file(GLOB QSCI_SRC CONFIGURE_DEPENDS
    "${qscintilla_SOURCE_DIR}/src/*.cpp"
    "${qscintilla_SOURCE_DIR}/scintilla/src/*.cpp"
    "${qscintilla_SOURCE_DIR}/scintilla/lexers/*.cpp"
    "${qscintilla_SOURCE_DIR}/scintilla/lexlib/*.cpp")

file(GLOB QSCI_HDR CONFIGURE_DEPENDS
    "${qscintilla_SOURCE_DIR}/src/*.h"
    "${qscintilla_SOURCE_DIR}/src/Qsci/*.h")

add_library(qscintilla2_qt6 SHARED ${QSCI_SRC} ${QSCI_HDR})

set_target_properties(qscintilla2_qt6 PROPERTIES
    AUTOMOC ON
    DEBUG_POSTFIX "d")

target_compile_definitions(qscintilla2_qt6
    PRIVATE SCINTILLA_QT SCI_LEXER INCLUDE_DEPRECATED_FEATURES QSCINTILLA_MAKE_DLL)

target_include_directories(qscintilla2_qt6
    PUBLIC  "${qscintilla_SOURCE_DIR}/src"
    PRIVATE "${qscintilla_SOURCE_DIR}/scintilla/include"
            "${qscintilla_SOURCE_DIR}/scintilla/lexlib"
            "${qscintilla_SOURCE_DIR}/scintilla/src")

target_link_libraries(qscintilla2_qt6
    PUBLIC Qt6::Widgets Qt6::PrintSupport)

set(QSCI_TARGET_NAME qscintilla2_qt6)

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/assets/util/ProgrammingLanguages.yml"
    "${CMAKE_CURRENT_BINARY_DIR}/assets/util/ProgrammingLanguages.yml"
    COPYONLY
)

add_subdirectory(
    "${CMAKE_SOURCE_DIR}/external/SDL"
    "${CMAKE_CURRENT_BINARY_DIR}/external/SDL"
    EXCLUDE_FROM_ALL
)

add_subdirectory(engine)
add_subdirectory(studio)
add_subdirectory(client)

include_directories(SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/external)