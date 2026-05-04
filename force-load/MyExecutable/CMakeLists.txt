add_executable(MyExecutable
    Main.cpp
)

if(NOT EMSCRIPTEN)
    target_link_libraries(MyExecutable PRIVATE MySharedLib::MySharedLib)
endif()

target_link_libraries(MyExecutable PRIVATE MyStaticLib::MyStaticLib)

if (IOS)
    set_target_properties(MyExecutable PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_GUI_IDENTIFIER "MyExecutable"
        MACOSX_BUNDLE_BUNDLE_NAME "MyExecutable"

        # Look for dylibs inside .app bundle Frameworks folder
        BUILD_RPATH "@executable_path/Frameworks"
        INSTALL_RPATH "@executable_path/Frameworks"
        # Embed framework(s) into .app bundle
        XCODE_EMBED_FRAMEWORKS MySharedLibInner
    )
endif()
