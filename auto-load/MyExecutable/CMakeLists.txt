add_executable(MyExecutable
    Main.cpp
)

target_link_libraries(MyExecutable PRIVATE MySharedLib)
target_link_libraries(MyExecutable PRIVATE MyStaticLib)
