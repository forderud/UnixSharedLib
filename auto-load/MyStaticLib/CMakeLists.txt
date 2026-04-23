add_library(MyStaticLib STATIC
            MyStaticLib.cpp
)

# Symbol visibility setup
set_target_properties(MyStaticLib PROPERTIES
    COMPILE_FLAGS "-fvisibility=hidden -fvisibility-inlines-hidden")

# fore clients to link in entire static lib
#target_link_options(MyStaticLib INTERFACE "-force_load") # on APPLE
#target_link_options(MyStaticLib INTERFACE "LINKER:--whole-archive") # -Wl,--no-whole-archive
#target_link_options(MyStaticLib INTERFACE "$<LINK_LIBRARY:WHOLE_ARCHIVE,libMyStaticLib.a>")
