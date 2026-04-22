add_executable(MyExecutable
    Main.cpp
)

target_link_libraries(MyExecutable PRIVATE MySharedLib)

#target_link_libraries(MyExecutable PRIVATE MyStaticLib)
if (APPLE)
  target_link_libraries(MyExecutable PRIVATE "-force_load" MyStaticLib)
  #target_link_libraries(MyExecutable PRIVATE "$<LINK_LIBRARY:WHOLE_ARCHIVE,MyStaticLib>") # WHOLE_ARCHIVE available in CMake >=3.24
else()
  target_link_libraries(MyExecutable PRIVATE "-Wl,--whole-archive" MyStaticLib "-Wl,--no-whole-archive")
endif()
