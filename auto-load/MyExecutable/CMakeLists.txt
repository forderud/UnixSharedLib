add_executable(MyExecutable
    Main.cpp
)

target_link_libraries(MyExecutable PRIVATE MySharedLib)

#target_link_libraries(MyExecutable PRIVATE MyStaticLib)
#target_link_libraries(MyExecutable PRIVATE "-Wl,--whole-archive" MyStaticLib "-Wl,--no-whole-archive")
target_link_libraries(MyExecutable PRIVATE "$<LINK_LIBRARY:WHOLE_ARCHIVE,MyStaticLib>") # WHOLE_ARCHIVE available in CMake >=3.24
