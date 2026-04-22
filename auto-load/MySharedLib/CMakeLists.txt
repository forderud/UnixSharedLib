add_library(MySharedLib SHARED
            MySharedLib.cpp
)

# Symbol visibility setup
set_target_properties(MySharedLib PROPERTIES
    COMPILE_FLAGS "-fvisibility=hidden -fvisibility-inlines-hidden")

# Enable dead code elimination
if (APPLE)
  target_link_options(MySharedLib PRIVATE "-dead_strip")
else()
  target_compile_options(MySharedLib PRIVATE -ffunction-sections -fdata-sections)
  target_link_options(MySharedLib PRIVATE -Wl,--gc-sections)
endif()
