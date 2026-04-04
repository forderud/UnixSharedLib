add_library(MyDynFramework SHARED
            MyDynFramework.cpp
            MyDynFramework.hpp
)

set_target_properties(MyDynFramework PROPERTIES
  FRAMEWORK TRUE
  FRAMEWORK_VERSION C
  MACOSX_FRAMEWORK_IDENTIFIER com.MyDynFramework
  MACOSX_FRAMEWORK_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist
  VERSION 1.0.0   # "current version" in semantic format in Mach-O binary file
  SOVERSION 1.0.0 # "compatibility version" in semantic format in Mach-O binary file
  PUBLIC_HEADER MyDynFramework.hpp
  XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer"
)

# Symbol visibility setup, COMPILE_FLAGS only affect C++ so for Objective C we
# have to use XCODE_ATTRIBUTE_OTHER_CFLAGS.
set_target_properties(MyDynFramework PROPERTIES
    COMPILE_FLAGS "-fvisibility=hidden -fvisibility-inlines-hidden"
    XCODE_ATTRIBUTE_OTHER_CFLAGS "-fvisibility=hidden -fvisibility-inlines-hidden")
