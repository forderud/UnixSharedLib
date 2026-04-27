add_library(MyStaticLibInner STATIC
    MyStaticLib.cpp
)

# wrap static library to force clients to link in entire static lib
# Based on https://stackoverflow.com/questions/53071878/using-whole-archive-linker-option-with-cmake-and-libraries-with-other-library
add_library(MyStaticLib INTERFACE)
set_target_properties(MyStaticLib PROPERTIES LINKER_LANGUAGE CXX)

#TODO: Switch to WHOLE_ARCHIVE when upgrading to CMake >=3.24
# Unable to use target_link_options(MyStaticLib [BEFORE] INTERFACE LINKER:<flags>) since the flags aren't applied immediately before/after the library.
if(APPLE)
    target_link_libraries(MyStaticLib INTERFACE
        "-Wl,-force_load"
        MyStaticLibInner
    )
else()
    target_link_libraries(MyStaticLib INTERFACE
        "-Wl,--whole-archive"
        MyStaticLibInner
        "-Wl,--no-whole-archive"
    )
endif()
