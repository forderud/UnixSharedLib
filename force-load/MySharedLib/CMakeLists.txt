add_library(MySharedLibInner SHARED
    MySharedLib.cpp
)

# wrap shared library to force clients to force-link to it, despite it being unreferenced
# Based on https://stackoverflow.com/questions/53071878/using-whole-archive-linker-option-with-cmake-and-libraries-with-other-library
add_library(MySharedLib INTERFACE)
set_target_properties(MySharedLib PROPERTIES LINKER_LANGUAGE CXX)

# Unable to use target_link_options(MySharedLib [BEFORE] INTERFACE LINKER:<flags>) since the flags aren't applied immediately before/after the library.
if(APPLE)
    target_link_libraries(MySharedLib INTERFACE "$<LINK_LIBRARY:NEEDED_LIBRARY,MySharedLibInner>")
else()
    target_link_libraries(MySharedLib INTERFACE
        "-Wl,--no-as-needed"
        MySharedLibInner
        "-Wl,--as-needed"
    )
endif()
