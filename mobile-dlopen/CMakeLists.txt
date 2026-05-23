cmake_minimum_required(VERSION 3.22)
set(CMAKE_CXX_STANDARD 17)

project(TestProject)

set(VERSION "0.1")

# enable optimizations
add_compile_options(-O3)

# use dlopen to load shared libs at runtime
add_compile_definitions(USE_DLOPEN)

if(APPLE)
    # prevent cmake install from breaking code signing
    set(CMAKE_BUILD_WITH_INSTALL_RPATH ON)
endif()

# eliminate dead code
if (APPLE)
    add_link_options("LINKER:-dead_strip")
    add_link_options("LINKER:-dead_strip_dylibs")
else()
    add_compile_options("-ffunction-sections" "-fdata-sections")
    add_link_options("LINKER:--gc-sections")
endif()

# hidden visibility by default
add_compile_options("-fvisibility=hidden" "-fvisibility-inlines-hidden")

# OpenGL detection
if(ANDROID)
    find_library(GLES3_LIBRARY GLESv3)
elseif (IOS)
    # OpenGL ES3 check requre CMake 3.27
    #find_package(OpenGL REQUIRED COMPONENTS GLES3) # GLES2 also fail
    find_library(OPENGLES3_LIBRARY OpenGLES)
else()
    find_package(OpenGL REQUIRED COMPONENTS OpenGL)
endif()

add_subdirectory(MySharedLib)

if (APPLE OR ANDROID)
    # macOS, iOS & Android (Linux doesn´t support app bundles)
    add_subdirectory(MobileDlopen)
endif()

if (APPLE)
    # macOS & iOS
    add_subdirectory(ParseMach)
else()
    # Linux & Android
    add_subdirectory(ParseELF)
endif()
