add_executable(MyExecutable
    Main.cpp
)

target_link_libraries(MyExecutable PRIVATE "$<LINK_LIBRARY:FRAMEWORK,MyDynFramework>")
