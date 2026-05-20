if(ANDROID)
    add_library(MobileDlopen SHARED)
else()
    add_executable(MobileDlopen)
endif()

target_sources(MobileDlopen PRIVATE
    Main.cpp
)

if (APPLE)
    set_target_properties(MobileDlopen PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_GUI_IDENTIFIER "MobileDlopen"
        MACOSX_BUNDLE_BUNDLE_NAME "MobileDlopen"
        XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "MobileDlopen" # avoid Xcode warning
        MACOSX_BUNDLE_BUNDLE_VERSION "${VERSION}"
        MACOSX_BUNDLE_SHORT_VERSION_STRING "${VERSION}"
    )
endif()

if(ANDROID)
    # Build native_app_glue
    set(APP_GLUE_DIR ${ANDROID_NDK}/sources/android/native_app_glue)
    add_library(native_app_glue STATIC
        ${APP_GLUE_DIR}/android_native_app_glue.c)
    target_include_directories(native_app_glue INTERFACE ${APP_GLUE_DIR})

    target_link_libraries(MobileDlopen PRIVATE android log native_app_glue)

    # prevent linker from stripping away the entry point
    target_link_options(MobileDlopen PRIVATE "-u" "ANativeActivity_onCreate")
endif()

if (APPLE)
    find_library(CORE_FOUNDATION CoreFoundation)
    target_link_libraries(MobileDlopen PRIVATE ${CORE_FOUNDATION})

    target_link_libraries(MobileDlopen PRIVATE "$<LINK_LIBRARY:FRAMEWORK,MySharedLib>")

    if(IOS)
        set_target_properties(MobileDlopen PROPERTIES
            # Look for dylibs inside .app bundle Frameworks folder
            INSTALL_RPATH "@executable_path/Frameworks"
            # Embed framework(s) into .app bundle
            XCODE_EMBED_FRAMEWORKS MySharedLib
            XCODE_EMBED_FRAMEWORKS_CODE_SIGN_ON_COPY YES
        )
    else()
        set_target_properties(MobileDlopen PROPERTIES
            # Look for dylibs inside .app bundle Frameworks folder
            INSTALL_RPATH "@executable_path/../Frameworks"
            # Embed framework(s) into .app bundle
            XCODE_EMBED_FRAMEWORKS MySharedLib
            XCODE_EMBED_FRAMEWORKS_CODE_SIGN_ON_COPY YES
        )
    endif()
else()
    target_link_libraries(MobileDlopen PRIVATE MySharedLib)

    set_target_properties(MobileDlopen PROPERTIES
        # Look for .so files in same directory as executable
        INSTALL_RPATH "\$ORIGIN"
    )
endif()

# install rules
install(
    TARGETS MobileDlopen
    DESTINATION .
    BUNDLE DESTINATION .
)
