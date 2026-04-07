#include <iostream>
#include <string_view>
#include "incbin.h"

#define GetEmbeddedFile(NAME) { \
    (std::string_view((const char*)g##NAME##Data, g##NAME##Size)) \
}


INCBIN(EmbedExample, "embed_example.txt");
//INCTXT_EXTERN(EmbedExample);


int main() {
    std::string_view file = GetEmbeddedFile(EmbedExample);
    printf("Size   : %u\n", file.size());
    printf("Content: %.*s\n", (int)file.size(), file.data()); // specify size since file content is not null-terminated
    return 0;
}
