#include <iostream>

extern "C" {
    //char _binary_embed_example_txt_start[32]; // start of embedded ASCII file
    //char _binary_embed_example_txt_end[1];
}

int InitializationTask() {
    printf("Library loading.\n");
    return 42;
}

//__attribute__((used))
static int g_forceLink = InitializationTask();

extern "C"
void MyLibFunction() {
    printf("Hi from myLib!\n");
}
