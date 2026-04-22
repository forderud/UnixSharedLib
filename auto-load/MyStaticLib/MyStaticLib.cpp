#include <cstdio>

__attribute__((constructor))
static void init_function () {
    printf("MyStaticLib loaded.\n");
}
