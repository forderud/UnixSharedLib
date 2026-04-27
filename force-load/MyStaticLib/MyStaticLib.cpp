#include <cstdio>

// automatically called at startup
__attribute__((constructor))
void mystaticlib_init () {
    printf("  MyStaticLib loaded.\n");
}

// never called
__attribute__((used))
void mystaticlib_used () {
    printf("  mystaticlib_used called.\n");
}

// never called
void mystaticlib_hidden () {
    printf("  mystaticlib_hidden called.\n");
}

// exported function
__attribute__((visibility("default")))
void mystaticlib_public () {
    printf("  mystaticlib_public called.\n");
}
