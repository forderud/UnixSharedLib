add_executable(MyExecutable
    Main.cpp
)

add_dependencies(MyExecutable MyDynFramework)

# Link the framework to the app
set_target_properties(MyExecutable PROPERTIES
    XCODE_ATTRIBUTE_OTHER_LDFLAGS "${XCODE_ATTRIBUTE_OTHER_LDFLAGS} -framework MyDynFramework"
)

# Include framework headers, needed to make "Build" Xcode action work.
# "Archive" works fine just relying on default search paths as it has different
# build product output directory.
target_include_directories(MyExecutable PUBLIC 
    "MyDynFramework.framework"
)
