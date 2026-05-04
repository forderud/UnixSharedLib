add_executable(ParseMach
    ParseMach.cpp
)

if (IOS)
    set_target_properties(ParseMach PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_GUI_IDENTIFIER ParseMach
        XCODE_ATTRIBUTE_CODE_SIGN_STYLE "Automatic"
    )
endif()
