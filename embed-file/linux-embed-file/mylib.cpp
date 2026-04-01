#include <iostream>

extern "C" {
    char mylib_array[] = "MyLib text string.";
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
