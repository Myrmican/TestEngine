file(GLOB_RECURSE ENGINE_SOURCES
    CONFIGURE_DEPENDS
    "src/*.cpp"
    "include/*.h"
)

add_library(EngineCore STATIC ${ENGINE_SOURCES})

target_compile_features(EngineCore PUBLIC cxx_std_20)

target_include_directories(EngineCore PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/include/engine"
    "${CMAKE_SOURCE_DIR}/external"
    "${wasmtime_SOURCE_DIR}/include"
)