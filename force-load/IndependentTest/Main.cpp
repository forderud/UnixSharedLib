#include <cstdio>

extern void mystaticlib_function();
extern void mysharedlib_function();


int main() {
    printf("Program startup.\n");

    //mystaticlib_function();
    //mysharedlib_function();
    
    return 0;
}
