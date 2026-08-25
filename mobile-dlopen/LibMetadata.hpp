#pragma once
#include <cstdint>

#define LibMetadata_SYMBOL_NAME "LibMetadata"

/** Shared library metadata */
struct __attribute__((packed)) LibMetadataT {
    static constexpr char S_HEADER[] = "LIBM"; // magic value for validation
    char    header[4] = {S_HEADER[0], S_HEADER[1], S_HEADER[2], S_HEADER[3]};
    char    name[16]{};
    char    description[64]{};
    uint8_t version[4];
    uint8_t diagnostic : 1;
    uint8_t trusted : 1;

    // constexpr to allow constant-initialization into read-only section
    template <size_t N, size_t M>
    constexpr LibMetadataT(const char (&_name)[N], const char (&_description)[M],
                           uint8_t _version[4], bool _diagnostic, bool _trusted) : 
          version{_version[0], _version[1], _version[2], _version[3]},
          diagnostic(_diagnostic), trusted(_trusted)
    {
        static_assert(N <= sizeof(name), "name too long");
        for (size_t i = 0; i < N; ++i)
            name[i] = _name[i];

        static_assert(M <= sizeof(description), "description too long");
        for (size_t i = 0; i < M; ++i)
            description[i] = _description[i];
    }

    bool IsValid() const {
        //printf("Metadata header: %.4s\n", header);
        return strncmp(header, S_HEADER, sizeof(header)) == 0;
    }

    void Print() const {
        printf("  Library Name: %s\n", name);
        printf("  Description: %s\n", description);
        printf("  Version: %u.%u.%u.%u\n", version[0], version[1], version[2], version[3]);
        printf("  Diagnostic: %s\n", diagnostic ? "Yes" : "No");
        printf("  Trusted: %s\n", trusted ? "Yes" : "No");
    }
};
static_assert(sizeof(LibMetadataT) == 89, "LibMetadataT size mismatch");
