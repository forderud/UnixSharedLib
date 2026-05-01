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

# Use MyStaticLib headers/compile options for building StaticLibWrap, but do
# NOT propagate the link dependency: MyStaticLib will be physically embedded
# into the StaticLibWrap archive below, so downstream consumers don't need it.
target_include_directories(StaticLibWrap PRIVATE
    $<TARGET_PROPERTY:MyStaticLib,INTERFACE_INCLUDE_DIRECTORIES>
)
target_compile_definitions(StaticLibWrap PRIVATE
    $<TARGET_PROPERTY:MyStaticLib,INTERFACE_COMPILE_DEFINITIONS>
)
add_dependencies(StaticLibWrap MyStaticLib)

# Embed MyStaticLib into StaticLibWrap so the resulting archive is self-contained.
if(APPLE)
    # Apple's libtool can merge multiple static archives into one.
    add_custom_command(TARGET StaticLibWrap POST_BUILD
        COMMAND libtool -static
            -o $<TARGET_FILE:StaticLibWrap>
            $<TARGET_FILE:StaticLibWrap>
            $<TARGET_FILE:MyStaticLib>
        COMMENT "Embedding MyStaticLib into StaticLibWrap"
    )
else()
    # GNU ar MRI script: open StaticLibWrap, add all members of MyStaticLib, save.
    add_custom_command(TARGET StaticLibWrap POST_BUILD
        COMMAND cmake -E echo "CREATE $<TARGET_FILE:StaticLibWrap>"  > merge.mri
        COMMAND cmake -E echo "ADDLIB $<TARGET_FILE:StaticLibWrap>" >> merge.mri
        COMMAND cmake -E echo "ADDLIB $<TARGET_FILE:MyStaticLib>"   >> merge.mri
        COMMAND cmake -E echo "SAVE"                                >> merge.mri
        COMMAND cmake -E echo "END"                                 >> merge.mri
        COMMAND ar -M < merge.mri
        COMMAND ranlib $<TARGET_FILE:StaticLibWrap>
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Embedding MyStaticLib into StaticLibWrap"
        VERBATIM
    )
endif()

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
