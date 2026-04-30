include(CMakePackageConfigHelpers)

add_library(StaticLibWrap STATIC)

#TODO: Adopt FILE_SET HEADERS in CMake 3.23
target_sources(StaticLibWrap
  PRIVATE
    StaticLibWrap.cpp
)

set_target_properties(StaticLibWrap PROPERTIES PUBLIC_HEADER StaticLibWrap.hpp)

target_include_directories(StaticLibWrap PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(StaticLibWrap PRIVATE MyStaticLib)


# install rules
install(
    TARGETS StaticLibWrap
    EXPORT StaticLibWrapTargets
)
install(
    EXPORT StaticLibWrapTargets
    DESTINATION cmake
    NAMESPACE StaticLibWrap::
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/StaticLibWrapConfigVersion.cmake
    VERSION 1.0.0
    COMPATIBILITY SameMinorVersion
)
install(
    FILES
        StaticLibWrapConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/StaticLibWrapConfigVersion.cmake
    DESTINATION cmake
)
