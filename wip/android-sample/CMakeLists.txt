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

# Create main shared library
add_library(nativeapp-lib SHARED
    main.cpp)

# Link to android, log & glue libs.
target_link_libraries(nativeapp-lib PRIVATE android log native_app_glue)

# prevent linker from stripping away the entry point
target_link_options(nativeapp-lib PRIVATE "-u" "ANativeActivity_onCreate")
