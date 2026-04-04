#include <iostream>
#include <dlfcn.h>
// "mach-o" header folder path: /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/mach-o/
#include <mach-o/getsect.h> // for getsectiondata
#include <mach-o/ldsyms.h>  // for _mh_execute_header

const char INTERNAL_MYLIB_ARRAY[] = "This is an embedded file array.";

static int rcs_addr_handle = 0; // in-library variable

void print_embedded_file (const char* section_name) {
    // get image header from a global address
    Dl_info img_info = {};
    int img_index = dladdr(&rcs_addr_handle, &img_info);
    auto* header = (mach_header_64*)img_info.dli_fbase; // or &_mh_execute_header for an executable

    // access embedded binary file
    const char segment_name[] = "__TEXT"; // __TEXT for read-only data and __DATA for writable data
    unsigned long embed_example_size = 0;
    const uint8_t* embed_example_start = getsectiondata(header, segment_name, section_name, &embed_example_size);

    printf("Content of %s section (%u bytes):\n", section_name, (unsigned int)embed_example_size);
    printf("%.*s\n", (int)embed_example_size, embed_example_start); // specify size since file content is not null-terminated
    printf("\n");
    printf("Content of internal array (%u bytes):\n%s\n", (unsigned int)sizeof(INTERNAL_MYLIB_ARRAY), INTERNAL_MYLIB_ARRAY);
}
