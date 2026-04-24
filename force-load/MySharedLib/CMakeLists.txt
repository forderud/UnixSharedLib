add_library(MySharedLibInner SHARED
            MySharedLib.cpp
)

# Symbol visibility setup
set_target_properties(MySharedLibInner PROPERTIES
    COMPILE_FLAGS "-fvisibility=hidden -fvisibility-inlines-hidden")

# Enable dead code elimination
if (APPLE)
  target_link_options(MySharedLibInner PRIVATE "-dead_strip")
else()
  target_compile_options(MySharedLibInner PRIVATE -ffunction-sections -fdata-sections)
  target_link_options(MySharedLibInner PRIVATE "LINKER:--gc-sections")
endif()

# wrap shared library to force clients to force-link to it, despite it being unreferenced
# Based on https://stackoverflow.com/questions/53071878/using-whole-archive-linker-option-with-cmake-and-libraries-with-other-library
add_library(MySharedLib INTERFACE)
set_target_properties(MySharedLib PROPERTIES LINKER_LANGUAGE CXX)

# force client-side lib. loading
#TODO: Switch to NEEDED_LIBRARY when upgrading to CMake >=3.24
if(APPLE)
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
