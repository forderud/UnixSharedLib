#ifdef __ANDROID__
  #include <android/log.h>
  #define printf(...) __android_log_print(ANDROID_LOG_DEBUG, "MySharedLib", __VA_ARGS__)
#else
    #include <stdio.h>
#endif
#include "MySharedLib.hpp"


__attribute__ ((visibility ("default")))
void MySharedLib_function () {
    printf("sharedlib_function called.");
}
