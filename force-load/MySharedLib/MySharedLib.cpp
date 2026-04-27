#include <cstdio>

// automatically called during library loading
__attribute__((constructor))
__attribute__((visibility("default")))
void mysharedlib_init () {
    printf("  MySharedLib loaded.\n");
}

// never called
__attribute__((used))
void mysharedlib_used () {
    printf("  mysharedlib_used called.\n");
}

// never called
void mysharedlib_hidden () {
    printf("  mysharedlib_hidden called.\n");
}

// exported function
__attribute__((visibility("default")))
void mysharedlib_public () {
    printf("  mysharedlib_public called.\n");
}
