add_executable(ClientWindows WIN32
    src/main.cpp
    "include/util/System.h")

target_include_directories(ClientWindows PRIVATE
    ${CMAKE_SOURCE_DIR}/engine/include
    ${CMAKE_SOURCE_DIR}/client/include
    ${CMAKE_SOURCE_DIR}/client/targets/windows/include
)

target_link_libraries(ClientWindows PRIVATE
    EngineCore
)