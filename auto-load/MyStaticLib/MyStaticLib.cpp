#include <cstdio>

__attribute__((constructor))
static void init_function (const char* section_name) {
    printf("MyStaticLib loaded.\n");
}
