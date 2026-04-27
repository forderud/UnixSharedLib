add_library(MyStaticLibInner STATIC
    MyStaticLib.cpp
)

# wrap static library to force clients to link in entire static lib
# Based on https://stackoverflow.com/questions/53071878/using-whole-archive-linker-option-with-cmake-and-libraries-with-other-library
add_library(MyStaticLib INTERFACE)
set_target_properties(MyStaticLib PROPERTIES LINKER_LANGUAGE CXX)

# Unable to use target_link_options(MyStaticLib [BEFORE] INTERFACE LINKER:<flags>) since the flags aren't applied immediately before/after the library.
target_link_libraries(MyStaticLib INTERFACE "$<LINK_LIBRARY:WHOLE_ARCHIVE,MyStaticLibInner>")
