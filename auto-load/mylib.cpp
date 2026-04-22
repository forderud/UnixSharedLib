#include <iostream>

__attribute__ ((constructor))
void InitializationTask() {
    printf("Library loading.\n");
}

extern "C" __attribute__ ((visibility ("default")))
void MyLibFunction() {
    printf("Hi from myLib!\n");
}
