add_library(MySharedLib SHARED
  MySharedLib.cpp
  MySharedLib.hpp
)

if (APPLE)
  # package shared lib as a framework
  set_target_properties(MySharedLib PROPERTIES
    FRAMEWORK TRUE
    FRAMEWORK_VERSION A
    MACOSX_FRAMEWORK_IDENTIFIER MySharedLib
    VERSION 1.0.0   # "current version" in semantic format in Mach-O binary file
    SOVERSION 1.0.0 # "compatibility version" in semantic format in Mach-O binary file
    PUBLIC_HEADER   ${CMAKE_CURRENT_LIST_DIR}/MySharedLib.hpp
    XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer"
  )
else()
    # enable "#include <MySharedLib/MySharedLib.hpp>" in client projects
    target_include_directories(MySharedLib INTERFACE ..)
endif()

# Embed binary file as section in library
if (APPLE)
  # NOTE: section names cannot be longer than 16 chars
  target_link_options(MySharedLib PRIVATE -Wl,-sectcreate,__TEXT,embed_example,${CMAKE_CURRENT_SOURCE_DIR}/embed_example.txt)
else()
  # run linker to embed file
  set(OBJ_OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/embed_example.o")

  set(LD_EMULATION "")
  if (ANDROID)
    set(LD_EMULATION -m aarch64linux) # aarch64 regardless of host architecture
  endif()

  # convert embed_example.txt into an object file
  add_custom_command(
    OUTPUT ${OBJ_OUTPUT}
    COMMAND ${CMAKE_LINKER} -r -b binary -z noexecstack ${LD_EMULATION} embed_example.txt -o ${OBJ_OUTPUT}
    DEPENDS embed_example.txt
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  )
  
  target_sources(MySharedLib PRIVATE ${OBJ_OUTPUT})
endif()
