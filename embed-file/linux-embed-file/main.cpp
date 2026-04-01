#include <iostream>

extern "C" {
#if 0
    extern char _binary_embed_example_txt_start[]; // start of embedded ASCII file
    extern char _binary_embed_example_txt_end[];
#endif
    extern char mylib_array[];
    extern void MyLibFunction();
}

int main() {
#if 0
    size_t fileSize = _binary_embed_example_txt_end - _binary_embed_example_txt_start;
    printf("File size   : %zd\n", fileSize);
    printf("File content: ");
    printf("%.*s\n", (int)fileSize, _binary_embed_example_txt_start); // specify size since file content may not be null-terminated
    printf("\n");
#endif
    printf("myLib array: %s\n", mylib_array);
    MyLibFunction();
    return 0;
}
