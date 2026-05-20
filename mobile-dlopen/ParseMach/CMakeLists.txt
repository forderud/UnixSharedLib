add_executable(ParseMach
    ParseMach.cpp
)

if (IOS)
    set_target_properties(ParseMach PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_GUI_IDENTIFIER "ParseMach"
        MACOSX_BUNDLE_BUNDLE_NAME "ParseMach"
        XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "ParseMach" # avoid Xcode warning
    )
endif()

# install rules
install(
    TARGETS ParseMach
    DESTINATION .
)
