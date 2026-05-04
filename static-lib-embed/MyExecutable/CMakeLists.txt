add_executable(MyExecutable
    Main.cpp
)

target_link_libraries(MyExecutable PRIVATE StaticLibEmbed)

if (IOS)
    set_target_properties(MyExecutable PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_GUI_IDENTIFIER "MyExecutable"
        MACOSX_BUNDLE_BUNDLE_NAME "MyExecutable"
    )
endif()
