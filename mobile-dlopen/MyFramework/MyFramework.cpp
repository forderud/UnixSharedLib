#include <cassert>
#include <string>
#include "MyFramework.hpp"

#include "../LibMetadata.hpp"


// embed metadata in framework
__attribute__ ((visibility("default")))
#if defined(__APPLE__)
__attribute__ ((section("__TEXT,LibMetadata")))
#endif
extern const LibMetadataT LibMetadata(
    "MyFramework",
    "Sample shared framework.",
    (uint8_t[4]){1, 2, 3, 4},
    false,
    true
);


extern "C" __attribute__ ((visibility ("default")))
std::string_view LibraryFunction (const char* /*section_name*/) {
    static const char buffer[] = "Sample framework data"; // static to allow string_view return
    return std::string_view(buffer);
}
