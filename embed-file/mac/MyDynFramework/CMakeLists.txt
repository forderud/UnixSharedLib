add_library(MyDynFramework SHARED
            MyDynFramework.cpp
            MyDynFramework.hpp
)

set_target_properties(MyDynFramework PROPERTIES
  FRAMEWORK TRUE
  FRAMEWORK_VERSION A
  MACOSX_FRAMEWORK_IDENTIFIER com.MyDynFramework
  MACOSX_FRAMEWORK_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist
  VERSION 1.0.0   # "current version" in semantic format in Mach-O binary file
  SOVERSION 1.0.0 # "compatibility version" in semantic format in Mach-O binary file
  PUBLIC_HEADER   ${CMAKE_CURRENT_LIST_DIR}/MyDynFramework.hpp
  XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer"
)

# Embed binary file as section in library
# NOTE: section names cannot be longer than 16 chars
target_link_options(MyDynFramework PRIVATE -Wl,-sectcreate,__TEXT,embed_example,${CMAKE_CURRENT_SOURCE_DIR}/embed_example.txt)
