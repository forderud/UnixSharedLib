cmake_minimum_required(VERSION 3.22)
project("NativeApp")


if(ANDROID)
    # Build native_app_glue
    set(APP_GLUE_DIR ${ANDROID_NDK}/sources/android/native_app_glue)
    add_library(native_app_glue STATIC
        ${APP_GLUE_DIR}/android_native_app_glue.c)
    target_include_directories(native_app_glue INTERFACE ${APP_GLUE_DIR})
endif()

# Create shared lib for testing
add_library(MySharedLib SHARED
    MySharedLib.cpp)
if(ANDROID)
    target_link_libraries(MySharedLib PRIVATE log)
endif()

# Create main shared library
if(ANDROID)
    add_library(NativeApp SHARED)
else()
    add_executable(NativeApp)
endif()

target_sources(NativeApp PRIVATE
    main.cpp)

target_link_libraries(NativeApp PRIVATE MySharedLib)
if(ANDROID)
    # Link to android, log & glue libs.
    target_link_libraries(NativeApp PRIVATE android log native_app_glue)

    # prevent linker from stripping away the entry point
    target_link_options(NativeApp PRIVATE "-u" "ANativeActivity_onCreate")
endif()
