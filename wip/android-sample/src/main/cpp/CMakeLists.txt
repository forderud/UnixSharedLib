cmake_minimum_required(VERSION 3.22)
project("NativeApp")

# Use the shared C++ runtime (must be packaged alongside the .so).
set(ANDROID_STL c++_shared)

# Path to NDK's native_app_glue
set(APP_GLUE_DIR ${ANDROID_NDK}/sources/android/native_app_glue)

# Build native_app_glue
add_library(native_app_glue STATIC
    ${APP_GLUE_DIR}/android_native_app_glue.c)

# Create main shared library
add_library(nativeapp-lib SHARED
    main.cpp)

target_include_directories(nativeapp-lib PRIVATE ${APP_GLUE_DIR})

# Link to android, log and glue libs.
target_link_libraries(nativeapp-lib PRIVATE android log native_app_glue)
# prevent linker from stripping away the entry point
target_link_options(nativeapp-lib PRIVATE "-u" "ANativeActivity_onCreate")
