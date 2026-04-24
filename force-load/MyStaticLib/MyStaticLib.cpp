#include <cstdio>

// automatically called at startup
__attribute__((constructor))
static void init_function () {
    printf("  MyStaticLib loaded.\n");
}

// never called
__attribute__((used))
static void used_function () {
    printf("  MyStaticLib used_function called.\n");
}

__attribute__((visibility("default")))
void mystaticlib_function() {
    printf("  mystaticlib_function called.\n");
}
