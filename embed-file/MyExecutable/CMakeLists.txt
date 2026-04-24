add_executable(MyExecutable
    Main.cpp
)

if (APPLE)
    target_link_libraries(MyExecutable PRIVATE "$<LINK_LIBRARY:FRAMEWORK,MySharedLib>")
else()
    target_link_libraries(MyExecutable PRIVATE MySharedLib)
endif()
