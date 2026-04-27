#include <cstdio>

// automatically called at startup
__attribute__((constructor))
void init_mystaticlib () {
    printf("  MyStaticLib loaded.\n");
}

// never called
__attribute__((used))
void used_function () {
    printf("  MyStaticLib used_function called.\n");
}

// never called
void hidden_function () {
    printf("  MyStaticLib hidden_function called.\n");
}

__attribute__((visibility("default")))
void mystaticlib_function() {
    printf("  mystaticlib_function called.\n");
}
