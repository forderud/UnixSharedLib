add_executable(MyExecutable
    Main.cpp
)

if (APPLE)
    set_target_properties(MyExecutable PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_GUI_IDENTIFIER "MyExecutable"
        MACOSX_BUNDLE_BUNDLE_NAME "MyExecutable"
    )
endif()

if (APPLE)
    find_library(CORE_FOUNDATION CoreFoundation)
    target_link_libraries(MyExecutable PRIVATE ${CORE_FOUNDATION})

    target_link_libraries(MyExecutable PRIVATE "$<LINK_LIBRARY:FRAMEWORK,MySharedLib>")

    set_target_properties(MyExecutable PROPERTIES
        # Look for embedded dylibs inside the .app bundle's Frameworks folder
        BUILD_RPATH "@executable_path/Frameworks"
        INSTALL_RPATH "@executable_path/Frameworks"
        # Embed framework(s) into .app bundle
        XCODE_EMBED_FRAMEWORKS MySharedLib
        XCODE_EMBED_FRAMEWORKS_CODE_SIGN_ON_COPY YES
    )
else()
    target_link_libraries(MyExecutable PRIVATE MySharedLib)
endif()
