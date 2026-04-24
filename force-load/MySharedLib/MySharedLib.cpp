#include <cstdio>

// automatically called during library loading
__attribute__((constructor))
static void init_function () {
    printf("  MySharedLib loaded.\n");
}

// never called
__attribute__((used))
void used_function () {
    printf("  MySharedLib used_function called.\n");
}

// never called
void hidden_function () {
    printf("  MySharedLib hidden_function called.\n");
}

__attribute__((visibility("default")))
void mysharedlib_function() {
    printf("  mysharedlib_function called.\n");
}
