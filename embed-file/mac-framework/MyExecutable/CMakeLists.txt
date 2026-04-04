add_executable(MyExecutable
    Main.cpp
)

add_dependencies(MyExecutable MyDynFramework)

# Link the framework to the app
set_target_properties(MyExecutable PROPERTIES
    XCODE_ATTRIBUTE_OTHER_LDFLAGS "${XCODE_ATTRIBUTE_OTHER_LDFLAGS} -framework MyDynFramework"
)
