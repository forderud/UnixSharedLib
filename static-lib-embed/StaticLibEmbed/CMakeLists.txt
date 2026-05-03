include(CMakePackageConfigHelpers)

add_library(StaticLibEmbed STATIC)

#TODO: Adopt FILE_SET HEADERS in CMake 3.23
target_sources(StaticLibEmbed
  PRIVATE
    StaticLibEmbed.cpp
)

set_target_properties(StaticLibEmbed PROPERTIES PUBLIC_HEADER StaticLibEmbed.hpp)

target_include_directories(StaticLibEmbed PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

# Use MyStaticLib headers/compile options for building StaticLibEmbed, but do
# NOT propagate the link dependency: MyStaticLib will be physically embedded
# into the StaticLibEmbed archive below, so downstream consumers don't need it.
target_include_directories(StaticLibEmbed PRIVATE
    $<TARGET_PROPERTY:MyStaticLib,INTERFACE_INCLUDE_DIRECTORIES>
)
target_compile_definitions(StaticLibEmbed PRIVATE
    $<TARGET_PROPERTY:MyStaticLib,INTERFACE_COMPILE_DEFINITIONS>
)
add_dependencies(StaticLibEmbed MyStaticLib)

# Embed MyStaticLib into StaticLibEmbed so the resulting archive is self-contained.
if(APPLE)
    # Apple's libtool can merge multiple static archives into one.
    add_custom_command(TARGET StaticLibEmbed POST_BUILD
        COMMAND libtool -static
            -o $<TARGET_FILE:StaticLibEmbed>
            $<TARGET_FILE:StaticLibEmbed>
            $<TARGET_FILE:MyStaticLib>
            $<TARGET_FILE:InnerStaticLib>
        COMMENT "Embedding MyStaticLib into StaticLibEmbed"
    )
else()
    # GNU ar MRI script: open StaticLibEmbed, add MyStaticLib, save.
    add_custom_command(TARGET StaticLibEmbed POST_BUILD
        COMMAND cmake -E echo "CREATE $<TARGET_FILE:StaticLibEmbed>"  > merge.mri
        COMMAND cmake -E echo "ADDLIB $<TARGET_FILE:StaticLibEmbed>" >> merge.mri
        COMMAND cmake -E echo "ADDLIB $<TARGET_FILE:MyStaticLib>"    >> merge.mri
        COMMAND cmake -E echo "ADDLIB $<TARGET_FILE:InnerStaticLib>" >> merge.mri
        COMMAND cmake -E echo "SAVE"                                 >> merge.mri
        COMMAND cmake -E echo "END"                                  >> merge.mri
        COMMAND ${CMAKE_AR} -M < merge.mri
        COMMAND ranlib $<TARGET_FILE:StaticLibEmbed>
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Embedding MyStaticLib into StaticLibEmbed"
        VERBATIM
    )
endif()

# install rules
install(
    TARGETS StaticLibEmbed
    EXPORT StaticLibEmbedTargets
)
install(
    EXPORT StaticLibEmbedTargets
    DESTINATION cmake
    NAMESPACE StaticLibEmbed::
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/StaticLibEmbedConfigVersion.cmake
    VERSION 1.0.0
    COMPATIBILITY SameMinorVersion
)
install(
    FILES
        StaticLibEmbedConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/StaticLibEmbedConfigVersion.cmake
    DESTINATION cmake
)
