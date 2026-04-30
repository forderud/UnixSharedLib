add_executable(MyExecutable
    Main.cpp
)

target_link_libraries(MyExecutable PRIVATE StaticLibWrap)

if (IOS)
    set_target_properties(MyExecutable PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_GUI_IDENTIFIER MyExecutable
    )
endif()
