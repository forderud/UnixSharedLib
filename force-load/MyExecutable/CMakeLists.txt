add_executable(MyExecutable
    Main.cpp
)

if(NOT EMSCRIPTEN)
    target_link_libraries(MyExecutable PRIVATE MySharedLib::MySharedLib)
endif()

target_link_libraries(MyExecutable PRIVATE MyStaticLib::MyStaticLib)

if (IOS)
    set_target_properties(MyExecutable PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_GUI_IDENTIFIER "MyExecutable"
        MACOSX_BUNDLE_BUNDLE_NAME "MyExecutable"
    )
endif()
