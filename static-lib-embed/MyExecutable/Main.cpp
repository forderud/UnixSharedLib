#include <cstdio>
//#include <MyStaticLib.hpp>
#include <StaticLibEmbed.hpp>


int main() {
    printf("Program startup.\n");

    //mystaticlib_init();
    //mystaticlib_public();

    staticlibembed_public();

    return 0;
}
