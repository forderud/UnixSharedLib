add_executable(MyExecutable
    Main.cpp
)

if(NOT (EMSCRIPTEN OR IOS))
    target_link_libraries(MyExecutable PRIVATE MySharedLib::MySharedLib)
endif()

target_link_libraries(MyExecutable PRIVATE MyStaticLib::MyStaticLib)
