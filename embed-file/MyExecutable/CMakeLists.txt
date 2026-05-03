add_executable(MyExecutable
    Main.cpp
)

if (APPLE)
    set_target_properties(MyExecutable PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_GUI_IDENTIFIER MyExecutable
    )
endif()

if (APPLE)
    target_link_libraries(MyExecutable PRIVATE "$<LINK_LIBRARY:FRAMEWORK,MySharedLib>")

    find_library(CORE_FOUNDATION CoreFoundation)
    target_link_libraries(MyExecutable PRIVATE ${CORE_FOUNDATION})
else()
    target_link_libraries(MyExecutable PRIVATE MySharedLib)
endif()
