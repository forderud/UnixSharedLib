add_executable(MyExecutable
    Main.cpp
)

target_link_libraries(MyExecutable PRIVATE MyDynFramework)
#target_link_libraries(MyExecutable PRIVATE "-framework MyDynFramework")

# Link the framework to the app
#set_target_properties(MyExecutable PROPERTIES
#    XCODE_ATTRIBUTE_OTHER_LDFLAGS "${XCODE_ATTRIBUTE_OTHER_LDFLAGS} -framework MyDynFramework"
#)
