#pragma once
#include <cstdint>


struct __attribute__((packed)) LibMetadata {
    char    name[16];
    char    description[64];
    uint8_t version[4];
    uint8_t diagnostic : 1;
    uint8_t trusted : 1;

    void Print() const {
        printf("Library Name: %s\n", name);
        printf("Description: %s\n", description);
        printf("Version: %u.%u.%u.%u\n", version[0], version[1], version[2], version[3]);
        printf("Diagnostic: %s\n", diagnostic ? "Yes" : "No");
        printf("Trusted: %s\n", trusted ? "Yes" : "No");
    }
};


#ifdef EXPORT_LIB_METADATA

// embed sample metadata in shared library
//__attribute__ ((visibility("default"))) 
#if defined(__APPLE__)
__attribute__ ((section("__TEXT,LibMetadata")))
#endif
extern const LibMetadata LibMetadata = {
    "MySharedLib",
    "A sample ELF/Mach-O shared library.",
    {1, 2, 3, 4},
    true,
    true
};

#endif // EXPORT_LIB_METADATA
