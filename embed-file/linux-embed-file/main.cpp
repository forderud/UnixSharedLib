#include <iostream>

extern char _binary_embed_example_txt_start[]; // start of embedded ASCII file
extern char _binary_embed_example_txt_end[];


int main() {
    size_t fileSize = _binary_embed_example_txt_end - _binary_embed_example_txt_start;
    printf("File size   : %zd\n", fileSize);
    printf("File content: ");
    printf("%.*s\n", (int)fileSize, _binary_embed_example_txt_start); // specify size since file content may not be null-terminated
    return 0;
}
