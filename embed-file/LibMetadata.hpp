#pragma once

struct LibMetadata {
    char    name[16];
    char    description[64];
    uint8_t version[4];
    bool    diagnostic;
    bool trusted;
};
