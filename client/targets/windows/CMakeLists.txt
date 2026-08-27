add_executable(ClientWindows WIN32
    src/main.cpp
)

target_include_directories(ClientWindows PRIVATE
    ${CMAKE_SOURCE_DIR}/engine/include
    ${CMAKE_SOURCE_DIR}/client/include
)

target_link_libraries(ClientWindows PRIVATE
    EngineCore
)