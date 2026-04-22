add_executable(MyExecutable
    Main.cpp
)

target_link_libraries(MyExecutable PRIVATE MySharedLib)

#target_link_libraries(MyExecutable PRIVATE MyStaticLib)
target_link_libraries(MyExecutable PRIVATE "$<LINK_LIBRARY:WHOLE_ARCHIVE,MyStaticLib>")
