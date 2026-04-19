#pragma once

struct LibMetadata {
    char    name[16];
    char    description[64];
    uint8_t version[4];
    bool    diagnostic;
    bool    trusted;

    void Print() const {
        printf("Library Name: %s\n", name);
        printf("Description: %s\n", description);
        printf("Version: %u.%u.%u.%u\n", version[0], version[1], version[2], version[3]);
        printf("Diagnostic: %s\n", diagnostic ? "Yes" : "No");
        printf("Trusted: %s\n", trusted ? "Yes" : "No");
    }
};
