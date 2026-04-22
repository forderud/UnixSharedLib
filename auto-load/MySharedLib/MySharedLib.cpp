#include <cstdio>

__attribute__((constructor))
static void init_function (const char* section_name) {
    printf("MySharedLib loaded.\n");
}
