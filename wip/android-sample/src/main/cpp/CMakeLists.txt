cmake_minimum_required(VERSION 3.10.2)
project("NativeApp")

# Use the shared C++ runtime (must be packaged alongside the .so).
set(ANDROID_STL c++_shared)

# 1. Path to NDK's native_app_glue
set(APP_GLUE_DIR ${ANDROID_NDK}/sources/android/native_app_glue)

# 2. Build native_app_glue as a static library
add_library(native_app_glue STATIC
    ${APP_GLUE_DIR}/android_native_app_glue.c)

# 3. Create your main shared library
add_library(native-lib SHARED main.cpp)

# 4. Link required libraries: log, android, and the glue
target_include_directories(native-lib PRIVATE ${APP_GLUE_DIR})
target_link_libraries(native-lib native_app_glue android log)
target_link_options(native-lib PRIVATE "-u" "ANativeActivity_onCreate") # prevent linker from stripping the entry point
