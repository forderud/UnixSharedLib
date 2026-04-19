#include <iostream>
#include "../LibMetadata.hpp"


//__attribute__ ((visibility("default"))) 
/*const*/ LibMetadata LibMetadata = {
    "ABCABCABCABCABC",
    "A sample ELF shared library.",
    {1, 2, 3, 4},
    true,
    true
};

extern "C" {
    __attribute__ ((visibility ("default")))
    char mylib_array[] = "MyLib text string.";
}

int InitializationTask() {
    printf("Library loading.\n");
    return 42;
}

//__attribute__((used))
static int g_forceLink = InitializationTask();

extern "C" __attribute__ ((visibility ("default")))
void MyLibFunction() {
    printf("Hi from myLib!\n");
}
