file(GLOB_RECURSE STUDIO_SOURCES
    CONFIGURE_DEPENDS
    "src/*.cpp"
    "src/*.h"
    "include/*.h"
    "src/*.qss"
    "src/*.qrc"
)

add_executable(TestEngine ${STUDIO_SOURCES})

target_compile_features(TestEngine PUBLIC cxx_std_20)

target_compile_definitions(TestEngine PRIVATE
    -DUNICODE
    -D_UNICODE
    LANGUAGES_YML_PATH="${CMAKE_BINARY_DIR}/assets/util/ProgrammingLanguages.yml"
)

target_include_directories(TestEngine PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}/include" 
    "${CMAKE_SOURCE_DIR}/external/velopack_libc_1.2.0/include"
    "${qscintilla_SOURCE_DIR}/src"
    "${qscintilla_SOURCE_DIR}/scintilla/include"
    "${qscintilla_SOURCE_DIR}/scintilla/src"
    "${wasmtime_SOURCE_DIR}/include"
)

target_link_libraries(TestEngine PRIVATE 
    EngineCore
    "${CMAKE_SOURCE_DIR}/external/velopack_libc_1.2.0/lib-static/velopack_libc_win_x64_msvc.lib"
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
    get_target_property(QT_CORE_LIB_LOCATION Qt6::Core LOCATION)
    get_filename_component(QT_BIN_DIR "${QT_CORE_LIB_LOCATION}" DIRECTORY)
    
    add_custom_command(TARGET TestEngine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_FILE:QWindowKit::Core>"
            "$<TARGET_FILE_DIR:TestEngine>"

        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_FILE:QWindowKit::Widgets>"
            "$<TARGET_FILE_DIR:TestEngine>"

        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_FILE:${QSCI_TARGET_NAME}>"
            "$<TARGET_FILE_DIR:TestEngine>"
            
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${wasmtime_SOURCE_DIR}/lib/wasmtime.dll"
            "$<TARGET_FILE_DIR:TestEngine>"

        COMMAND "${QT_BIN_DIR}/windeployqt.exe" --no-compiler-runtime "$<TARGET_FILE:TestEngine>"
        
        WORKING_DIRECTORY "$<TARGET_FILE_DIR:TestEngine>"
        COMMENT "Copying DLLs and running windeployqt"
    )
endif()