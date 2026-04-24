add_executable(MyExecutable
    Main.cpp
)

if (APPLE)
    target_link_libraries(MyExecutable PRIVATE "$<LINK_LIBRARY:FRAMEWORK,MyDynFramework>")
else()
    target_link_libraries(MyExecutable PRIVATE MyDynFramework)
endif()
