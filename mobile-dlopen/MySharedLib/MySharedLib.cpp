#include <cassert>
#include <string>
#include <dlfcn.h>
#include <stdio.h>
#if defined(__APPLE__)
  // "mach-o" header folder path: /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/mach-o/
  #include <mach-o/getsect.h> // for getsectiondata
  #include <mach-o/ldsyms.h>  // for _mh_execute_header
#else
  extern "C" {
    extern char _binary_embed_example_txt_start[]; // start of embedded ASCII file
    extern char _binary_embed_example_txt_end[];
  }
#endif
#include "MySharedLib.hpp"

#define EMBED_SAMPLE_LIB_METADATA
#include "../LibMetadata.hpp"


static const char INTERNAL_MYLIB_ARRAY[] = "This is an embedded array.";


static std::string_view GetSectionData(const char* section_name) {
#if defined(__APPLE__)
    // get image header from a global address
    static int rcs_addr_handle = 0; // in-library variable
    Dl_info img_info = {};
    int ok = dladdr(&rcs_addr_handle, &img_info);
    assert(ok); (void)ok;
    auto* header = (mach_header_64*)img_info.dli_fbase; // or &_mh_execute_header for executables

    // access embedded file
    const char segment_name[] = "__TEXT"; // __TEXT for read-only data, __DATA for writable data
    unsigned long size = 0;
    const uint8_t* ptr = getsectiondata(header, segment_name, section_name, &size);
    return std::string_view((const char*)ptr, size);
#else
    size_t fileSize = _binary_embed_example_txt_end - _binary_embed_example_txt_start;
    return std::string_view(_binary_embed_example_txt_start, fileSize);
#endif
}

extern "C" __attribute__ ((visibility ("default")))
std::string_view print_embedded_file (const char* section_name) {
    auto section = GetSectionData(section_name);

    static char buffer[1024] = {}; // static to allow string_view return
    size_t i = 0;
    i += snprintf(buffer+i, sizeof(buffer)-i, "Hello from MySharedLib!\n");
    i += snprintf(buffer+i, sizeof(buffer)-i, "%s section (%u bytes): ", section_name, (unsigned int)section.size());
    i += snprintf(buffer+i, sizeof(buffer)-i, "%.*s\n", (int)section.size(), section.data()); // specify size since file content is not null-terminated
    i += snprintf(buffer+i, sizeof(buffer)-i, "Internal array (%u bytes): %s\n", (unsigned int)sizeof(INTERNAL_MYLIB_ARRAY), INTERNAL_MYLIB_ARRAY);
    assert(i <= sizeof(buffer));
    return std::string_view(buffer, i);
}
