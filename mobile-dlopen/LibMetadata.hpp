#pragma once
#include <cstdint>
#include <string>

#define LibMetadata_SYMBOL_NAME "LibMetadata"

/** Shared library metadata */
struct __attribute__((packed)) LibMetadataT {
    char    header[4] = {'L', 'I', 'B', 'M'}; // magic value for validation
    char    name[16];
    char    description[64];
    uint8_t version[4];
    uint8_t diagnostic : 1;
    uint8_t trusted : 1;

    LibMetadataT(std::string _name, std::string _description, uint8_t _version[4], bool _diagnostic, bool _trusted) {
        strncpy(name, _name.c_str(), sizeof(name)-1);
        strncpy(description, _description.c_str(), sizeof(description)-1);
        memcpy(version, _version, sizeof(version));
        diagnostic = _diagnostic;
        trusted = _trusted;
    }

    bool IsValid() const {
        return (header[0] == 'L') && (header[1] == 'I') && (header[2] == 'B') && (header[3] == 'M');
    }

    void Print() const {
        printf("  Library Name: %s\n", name);
        printf("  Description: %s\n", description);
        printf("  Version: %u.%u.%u.%u\n", version[0], version[1], version[2], version[3]);
        printf("  Diagnostic: %s\n", diagnostic ? "Yes" : "No");
        printf("  Trusted: %s\n", trusted ? "Yes" : "No");
    }
};


#ifdef EMBED_SAMPLE_LIB_METADATA

// embed sample metadata in shared library
__attribute__ ((visibility("default")))
#if defined(__APPLE__)
__attribute__ ((section("__TEXT,LibMetadata")))
#endif
extern const LibMetadataT LibMetadata = {
    "MySharedLib",
    "Sample shared library.",
    (uint8_t[4]){1, 2, 3, 4},
    false,
    true
};

#endif // EXPORT_LIB_METADATA
