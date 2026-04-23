add_library(MyStaticLibInner STATIC
            MyStaticLib.cpp
)

# Symbol visibility setup
set_target_properties(MyStaticLibInner PROPERTIES
    COMPILE_FLAGS "-fvisibility=hidden -fvisibility-inlines-hidden")

# fore clients to link in entire static lib
#target_link_options(MyStaticLib INTERFACE "-force_load") # on APPLE
#target_link_options(MyStaticLib INTERFACE "LINKER:--whole-archive") # -Wl,--no-whole-archive
#target_link_options(MyStaticLib INTERFACE "$<LINK_LIBRARY:WHOLE_ARCHIVE,libMyStaticLib.a>")

# wrapper static library to force whole-archive linking of MyStaticLibInner
# Based on https://stackoverflow.com/questions/53071878/using-whole-archive-linker-option-with-cmake-and-libraries-with-other-library
add_library(MyStaticLib INTERFACE)
set_target_properties(MyStaticLib PROPERTIES LINKER_LANGUAGE CXX)

if (APPLE)
    target_link_libraries(MyStaticLib INTERFACE
      -Wl,-force_load
      #$<TARGET_PROPERTY:MyStaticLibInner,INTERFACE_LINK_LIBRARIES>
      MyStaticLibInner
    )
else()
    target_link_libraries(MyStaticLib INTERFACE
      -Wl,--whole-archive
      #$<TARGET_PROPERTY:MyStaticLibInner,INTERFACE_LINK_LIBRARIES>
      MyStaticLibInner
      -Wl,--no-whole-archive
    )
endif()
