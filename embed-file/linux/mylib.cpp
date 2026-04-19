#include <iostream>

#define EXPORT_LIB_METADATA
#include "../LibMetadata.hpp"


extern "C" {
    __attribute__ ((visibility ("default")))
    char mylib_array[] = "MyLib text string.";
}

int InitializationTask() {
    printf("Library loading.\n");
    return 42;
}

//__attribute__((used))
static int g_forceLink = InitializationTask();

extern "C" __attribute__ ((visibility ("default")))
void MyLibFunction() {
    printf("Hi from myLib!\n");
}
