#include <cstdio>

// automatically called at startup
extern "C" // avoid name mangling
__attribute__((constructor))
__attribute__((visibility("default")))
size_t mystaticlib_init () {
    printf("  MyStaticLib loaded.\n");
    return 42;
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
extern "C" // avoid name mangling
__attribute__((visibility("default")))
void mystaticlib_public () {
    printf("  mystaticlib_public called.\n");
}

extern "C"
__attribute__((used))
size_t g_MyStaticLibForceLink = mystaticlib_init();
