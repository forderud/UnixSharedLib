add_executable(ParseMach
    ParseMach.cpp
)

if (IOS)
    set_target_properties(ParseMach PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_GUI_IDENTIFIER "ParseMach"
        MACOSX_BUNDLE_BUNDLE_NAME "ParseMach"
    )
endif()
