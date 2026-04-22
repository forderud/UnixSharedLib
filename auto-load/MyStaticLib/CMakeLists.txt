add_library(MyStaticLib STATIC
            MyStaticLib.cpp
)

# Symbol visibility setup
set_target_properties(MyStaticLib PROPERTIES
    COMPILE_FLAGS "-fvisibility=hidden -fvisibility-inlines-hidden")
