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
# Recursively collect all STATIC_LIBRARY targets reachable from `target`
# (including the target itself).
function(_collect_static_deps target out_var)
    set(_visited "")
    set(_stack "${target}")
    while(_stack)
        list(POP_FRONT _stack _cur)
        if(_cur IN_LIST _visited)
            continue()
        endif()
        list(APPEND _visited "${_cur}")
        if(NOT TARGET ${_cur})
            continue()
        endif()
        get_target_property(_type ${_cur} TYPE)
        if(NOT _type STREQUAL "STATIC_LIBRARY")
            continue()
        endif()
        foreach(_prop LINK_LIBRARIES INTERFACE_LINK_LIBRARIES)
            get_target_property(_libs ${_cur} ${_prop})
            if(_libs)
                list(APPEND _stack ${_libs})
            endif()
        endforeach()
    endwhile()
    # Drop non-target entries (e.g. raw -lfoo or generator expressions we can't resolve).
    set(_result "")
    foreach(_t IN LISTS _visited)
        if(TARGET ${_t})
            get_target_property(_type ${_t} TYPE)
            if(_type STREQUAL "STATIC_LIBRARY")
                list(APPEND _result "${_t}")
            endif()
        endif()
    endforeach()
    set(${out_var} "${_result}" PARENT_SCOPE)
endfunction()

_collect_static_deps(MyStaticLib EMBED_DEPS)
add_dependencies(StaticLibEmbed ${EMBED_DEPS})

# Build a list of $<TARGET_FILE:...> generator expressions for the merge step.
set(EMBED_FILES "")
foreach(_dep IN LISTS EMBED_DEPS)
    list(APPEND EMBED_FILES "$<TARGET_FILE:${_dep}>")
endforeach()

# Embed MyStaticLib (and its transitive static deps) into StaticLibEmbed
# so the resulting archive is self-contained.
if(APPLE)
    # Apple's libtool can merge multiple static archives into one.
    add_custom_command(TARGET StaticLibEmbed POST_BUILD
        COMMAND libtool -static
            -o $<TARGET_FILE:StaticLibEmbed>
            $<TARGET_FILE:StaticLibEmbed>
            ${EMBED_FILES}
        COMMAND_EXPAND_LISTS
        COMMENT "Embedding MyStaticLib (and transitive static deps) into StaticLibEmbed"
    )
else()
    # GNU ar MRI script: open StaticLibEmbed, add deps, save.
    set(_mri_addlibs "")
    foreach(_f IN LISTS EMBED_FILES)
        list(APPEND _mri_addlibs COMMAND cmake -E echo "ADDLIB ${_f}" >> merge.mri)
    endforeach()
    add_custom_command(TARGET StaticLibEmbed POST_BUILD
        COMMAND cmake -E echo "CREATE $<TARGET_FILE:StaticLibEmbed>"  > merge.mri
        COMMAND cmake -E echo "ADDLIB $<TARGET_FILE:StaticLibEmbed>" >> merge.mri
        ${_mri_addlibs}
        COMMAND cmake -E echo "SAVE"                                 >> merge.mri
        COMMAND cmake -E echo "END"                                  >> merge.mri
        COMMAND ar -M < merge.mri
        COMMAND ranlib $<TARGET_FILE:StaticLibEmbed>
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Embedding MyStaticLib (and transitive static deps) into StaticLibEmbed"
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
