#include <iostream>

extern "C" {
    extern void MyLibFunction();
}

int main() {
    MyLibFunction();
    return 0;
}
