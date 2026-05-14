#include <android/log.h>
#include "MySharedLib.hpp"


__attribute__ ((visibility ("default")))
void sharedlib_function () {
    __android_log_print(ANDROID_LOG_INFO, "MySharedLib", "sharedlib_function called.");
}
