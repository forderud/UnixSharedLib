#include <cstdio>

__attribute__((constructor))
static void init_function () {
    printf("MyStaticLib loaded.\n");
}

__attribute__((visibility("default")))
void mystaticlib_function() {
    printf("mystaticlib_function called.\n");
}
