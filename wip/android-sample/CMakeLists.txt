cmake_minimum_required(VERSION 3.22)
project("NativeApp")

# Build native_app_glue
set(APP_GLUE_DIR ${ANDROID_NDK}/sources/android/native_app_glue)
add_library(native_app_glue STATIC
    ${APP_GLUE_DIR}/android_native_app_glue.c)
target_include_directories(native_app_glue INTERFACE ${APP_GLUE_DIR})

# Create shared lib for testing
add_library(MySharedLib SHARED
    MySharedLib.cpp)
target_link_libraries(MySharedLib PRIVATE log)

# Create main shared library
add_library(NativeApp SHARED
    main.cpp)

# Link to android, log & glue libs.
target_link_libraries(NativeApp PRIVATE android log native_app_glue MySharedLib)

# prevent linker from stripping away the entry point
target_link_options(NativeApp PRIVATE "-u" "ANativeActivity_onCreate")
