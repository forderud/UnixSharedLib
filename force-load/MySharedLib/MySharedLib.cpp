#include <cstdio>

__attribute__((constructor))
static void init_function () {
    printf("  MySharedLib loaded.\n");
}

__attribute__((visibility("default")))
void mysharedlib_function() {
    printf("  mysharedlib_function called.\n");
}
