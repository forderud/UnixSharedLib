if(ANDROID)
    add_library(MobileDlopen SHARED)
else()
    add_executable(MobileDlopen)
endif()

target_sources(MobileDlopen PRIVATE
    Main.cpp
)

if (IOS)
    # UIKit host (Swift) that displays printf log messages on screen.
    enable_language(Swift)
    target_sources(MobileDlopen PRIVATE iOSApp.swift)
endif()

if (APPLE)
    # Asset catalog containing AppIcon (1024x1024 PNG in AppIcon.appiconset)
    target_sources(MobileDlopen PRIVATE Assets.xcassets)
    set_source_files_properties(Assets.xcassets PROPERTIES
        MACOSX_PACKAGE_LOCATION Resources
    )

    set_target_properties(MobileDlopen PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_GUI_IDENTIFIER "MobileDlopen"
        MACOSX_BUNDLE_BUNDLE_NAME "MobileDlopen"
        XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "MobileDlopen" # avoid Xcode warning
        MACOSX_BUNDLE_BUNDLE_VERSION "${VERSION}"
        MACOSX_BUNDLE_SHORT_VERSION_STRING "${VERSION}"
        XCODE_ATTRIBUTE_INSTALL_PATH "/Applications" # to avoid "archive" packaging problems
        MACOSX_BUNDLE_INFO_PLIST "Info.plist.in"
        # Tell asset compiler which icon set to use as the app icon
        XCODE_ATTRIBUTE_ASSETCATALOG_COMPILER_APPICON_NAME "AppIcon"
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
        # UIKit + Foundation for the on-screen log view in iOSApp.mm.
        find_library(UIKIT_FRAMEWORK UIKit)
        find_library(FOUNDATION_FRAMEWORK Foundation)
        target_link_libraries(MobileDlopen PRIVATE
            ${UIKIT_FRAMEWORK} ${FOUNDATION_FRAMEWORK})

        set_target_properties(MobileDlopen PROPERTIES
            # Look for dylibs inside .app bundle Frameworks folder
            INSTALL_RPATH "@executable_path/Frameworks"
            # Embed framework(s) into .app bundle
            XCODE_EMBED_FRAMEWORKS MySharedLib
            XCODE_EMBED_FRAMEWORKS_CODE_SIGN_ON_COPY YES
            # Auto-generate a launch screen so the app actually launches on iOS 14+.
            XCODE_ATTRIBUTE_INFOPLIST_KEY_UILaunchScreen_Generation YES
            # Target iPhone + iPad.
            XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY "1,2"
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
