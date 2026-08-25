include(CMakePackageConfigHelpers)

add_library(MyFramework SHARED
  MyFramework.cpp
  MyFramework.hpp
)

if (APPLE)
  # package shared lib as a framework
  set_target_properties(MyFramework PROPERTIES
    FRAMEWORK TRUE
    FRAMEWORK_VERSION A
    MACOSX_FRAMEWORK_IDENTIFIER "MyFramework"
    XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "MyFramework" # avoid Xcode warning
    VERSION 1.0.0   # "current version" in semantic format in Mach-O binary file
    SOVERSION 1.0.0 # "compatibility version" in semantic format in Mach-O binary file
    MACOSX_FRAMEWORK_BUNDLE_VERSION "${VERSION}" # to avoid app store validation error
    MACOSX_FRAMEWORK_SHORT_VERSION_STRING "${VERSION}" # to avoid app store validation error
    PUBLIC_HEADER   ${CMAKE_CURRENT_LIST_DIR}/MyFramework.hpp
    XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer"
    XCODE_ATTRIBUTE_SKIP_INSTALL YES # to avoid "archive" packaging problems
  )
else()
    # enable "#include <MyFramework/MyFramework.hpp>" in client projects
    target_include_directories(MyFramework INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
        $<INSTALL_INTERFACE:include>
    )
endif()

if(ANDROID)
    target_link_libraries(MyFramework PRIVATE log)
endif()

# install rules
install(
    TARGETS MyFramework
    EXPORT MyFrameworkTargets
    DESTINATION .
    FRAMEWORK DESTINATION .
)
install(
    EXPORT MyFrameworkTargets
    DESTINATION cmake
    NAMESPACE MyFramework::
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/MyFrameworkConfigVersion.cmake
    VERSION 1.0.0
    COMPATIBILITY SameMinorVersion
)
install(
    FILES
        MyFrameworkConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/MyFrameworkConfigVersion.cmake
    DESTINATION cmake
)
