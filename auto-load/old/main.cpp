#include <iostream>

extern "C" {
    extern void MyLibFunction();
}

int main() {
    // don't call library
    //MyLibFunction();

    return 0;
}
