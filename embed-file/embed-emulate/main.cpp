#include <string>

const unsigned char file[] = {
  #embed "embed_example.txt"
};

// WIP: Attempt on creating a #embed wrapper
#define INCBIN(FILENAME) \
  const unsigned char data_##FILENAME[] = { \
  };

INCBIN(embed_example_txt)


int main() {
    const unsigned char* ptr = data_embed_example_txt;
 
    printf("Size   : %lu\n", sizeof(file));
    printf("Content: %.*s\n", (int)sizeof(file), file); // specify size since file content is not null-terminated
    return 0;
}
