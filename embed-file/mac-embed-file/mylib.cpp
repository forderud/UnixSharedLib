#include <iostream>
// "mach-o" header folder path: /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/mach-o/
#include <mach-o/getsect.h> // for getsectiondata
#include <mach-o/ldsyms.h>  // for _mh_execute_header


void print_embedded_file (const char* section_name) {
#if 0
    // access embedded binary file
    const char segment_name[] = "__TEXT"; // __TEXT for read-only data and __DATA for writable data
    unsigned long embed_example_size = 0;
    const uint8_t* embed_example_start = getsectiondata(&_mh_execute_header, segment_name, section_name, &embed_example_size);

    printf("Content of %s section (%u bytes):\n", section_name, (unsigned int)embed_example_size);
    printf("%.*s\n", (int)embed_example_size, embed_example_start); // specify size since file content is not null-terminated
#endif
}
