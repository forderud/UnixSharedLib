#include <string>

const unsigned char file[] = {
  #embed "embed_example.txt"
};


int main() {
    printf("Size   : %lu\n", sizeof(file));
    printf("Content: %.*s\n", (int)sizeof(file), file); // specify size since file content is not null-terminated
    return 0;
}
