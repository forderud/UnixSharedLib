add_library(MyStaticLib STATIC
            MyStaticLib.cpp
)

# Symbol visibility setup
set_target_properties(MyStaticLib PROPERTIES
    COMPILE_FLAGS "-fvisibility=hidden -fvisibility-inlines-hidden")

# Force clients that link MyStaticLib to pull in the WHOLE archive,
# without each client having to wrap the target with $<LINK_LIBRARY:WHOLE_ARCHIVE,...>.
# When a consumer does `target_link_libraries(foo PRIVATE MyStaticLib)`, CMake
# replaces the `MyStaticLib` reference with the generator expression below.
# Requires CMake >= 3.24.
set_property(TARGET MyStaticLib
    PROPERTY INTERFACE_LINK_LIBRARIES_DIRECT
        "$<LINK_LIBRARY:WHOLE_ARCHIVE,MyStaticLib>"
)
