include(CMakePackageConfigHelpers)

add_library(MySharedLibInner SHARED)

target_sources(MySharedLibInner
  PRIVATE
    MySharedLib.cpp

  PUBLIC
    FILE_SET HEADERS # introduced in CMake 3.23
    FILES
      MySharedLib.hpp
)

# wrap shared library to force clients to force-link to it, despite it being unreferenced
# Based on https://stackoverflow.com/questions/53071878/using-whole-archive-linker-option-with-cmake-and-libraries-with-other-library
add_library(MySharedLib INTERFACE)
set_target_properties(MySharedLib PROPERTIES LINKER_LANGUAGE CXX)

# Unable to use target_link_options(MySharedLib [BEFORE] INTERFACE LINKER:<flags>) since the flags aren't applied immediately before/after the library.
if(APPLE)
    #TODO: Switch to NEEDED_LIBRARY when upgrading to CMake >=3.24
    target_link_libraries(MySharedLib INTERFACE
        "-Wl,-needed_library"
        MySharedLibInner # libMySharedLib.dylib
    )
else()
    target_link_libraries(MySharedLib INTERFACE
        "-Wl,--no-as-needed"
        MySharedLibInner
        "-Wl,--as-needed"
    )
endif()

install(
  TARGETS MySharedLib MySharedLibInner
  EXPORT MySharedLibTargets
  FILE_SET HEADERS
)

install(
  EXPORT MySharedLibTargets
  DESTINATION cmake
  NAMESPACE MySharedLib::
)

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/MySharedLibConfigVersion.cmake
  VERSION 1.0.0
  COMPATIBILITY SameMinorVersion
)

install(
  FILES
    MySharedLibConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/MySharedLibConfigVersion.cmake
  DESTINATION cmake
)
