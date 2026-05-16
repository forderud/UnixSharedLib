if(ANDROID)
    add_library(MyExecutable SHARED)
else()
    add_executable(MyExecutable)
endif()

target_sources(MyExecutable PRIVATE
    Main.cpp
)

if (APPLE)
    set_target_properties(MyExecutable PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_GUI_IDENTIFIER "MyExecutable"
        MACOSX_BUNDLE_BUNDLE_NAME "MyExecutable"
    )
endif()

if(ANDROID)
    # Build native_app_glue
    set(APP_GLUE_DIR ${ANDROID_NDK}/sources/android/native_app_glue)
    add_library(native_app_glue STATIC
        ${APP_GLUE_DIR}/android_native_app_glue.c)
    target_include_directories(native_app_glue INTERFACE ${APP_GLUE_DIR})

    target_link_libraries(MyExecutable PRIVATE android log native_app_glue)

    # prevent linker from stripping away the entry point
    target_link_options(MyExecutable PRIVATE "-u" "ANativeActivity_onCreate")
endif()

if (APPLE)
    find_library(CORE_FOUNDATION CoreFoundation)
    target_link_libraries(MyExecutable PRIVATE ${CORE_FOUNDATION})

    target_link_libraries(MyExecutable PRIVATE "$<LINK_LIBRARY:FRAMEWORK,MySharedLib>")

    if(IOS)
        set_target_properties(MyExecutable PROPERTIES
            # Look for dylibs inside .app bundle Frameworks folder
            INSTALL_RPATH "@executable_path/Frameworks"
            # Embed framework(s) into .app bundle
            XCODE_EMBED_FRAMEWORKS MySharedLib
            XCODE_EMBED_FRAMEWORKS_CODE_SIGN_ON_COPY YES
        )
    else()
        set_target_properties(MyExecutable PROPERTIES
            # Look for dylibs inside .app bundle Frameworks folder
            INSTALL_RPATH "@executable_path/../Frameworks"
            # Embed framework(s) into .app bundle
            XCODE_EMBED_FRAMEWORKS MySharedLib
            XCODE_EMBED_FRAMEWORKS_CODE_SIGN_ON_COPY YES
        )
    endif()
else()
    target_link_libraries(MyExecutable PRIVATE MySharedLib)

    set_target_properties(MyExecutable PROPERTIES
        # Look for .so files in same directory as executable
        INSTALL_RPATH "\$ORIGIN"
    )
endif()

# install rules
install(
    TARGETS MyExecutable
    DESTINATION .
    BUNDLE DESTINATION .
)
