#include <cstdio>
//#include <MyStaticLib.hpp>
#include <StaticLibWrap.hpp>


int main() {
    printf("Program startup.\n");

    //mystaticlib_init();
    //mystaticlib_public();

    staticlibwrap_public();

    return 0;
}
