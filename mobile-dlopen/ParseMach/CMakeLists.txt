add_executable(ParseMach
    Main.cpp
)

if (IOS)
    set_target_properties(ParseMach PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_GUI_IDENTIFIER "ParseMach"
        MACOSX_BUNDLE_BUNDLE_NAME "ParseMach"
        XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "ParseMach" # avoid Xcode warning
    )
endif()

if(APPLE)
    if (IOS)
        target_link_libraries(ParseMach PRIVATE ${GLES3_LIBRARY})
    else()
        target_link_libraries(ParseMach PRIVATE OpenGL::GL)
    endif()
endif()

# install rules
install(
    TARGETS ParseMach
    DESTINATION .
)
