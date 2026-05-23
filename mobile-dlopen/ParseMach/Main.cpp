#include <cassert>
#include "ParseMach.hpp"
#include "../FileMap.hpp"
#include "../LibMetadata.hpp"

#if TARGET_OS_IPHONE
    #include <OpenGLES/ES3/gl.h> // OpenGL ES 3.0 header test
#endif

int main(int argc, char **argv) {
  if (argc < 2) {
    printf("usage: %s <macho-binary>\n", argv[0]);
    return 1;
  }

#if 0
  printf("Parsing Mach-O symbols in %s...\n", argv[1]);
#endif
  FileMap file(argv[1]);

  auto& hdr = *(const mach_header_64*)file.ptr();
  if (hdr.magic != MH_MAGIC_64) {
    printf("Target is not a valid Mach-O 64-bit executable\n");
    return 1;
  }

#if 0
  printf("Mach-O header: cputype=0x%x, cpusubtype=0x%x, filetype=0x%x, ncmds=%u, sizeofcmds=%u\n",
         hdr.cputype, hdr.cpusubtype, hdr.filetype, hdr.ncmds, hdr.sizeofcmds);
  printf("\n");
#endif

  if ((hdr.filetype != MH_EXECUTE) && (hdr.filetype != MH_DYLIB)) {
    printf("Target is not an Mach-O executable or dynamic library.\n");
    return 1;
  }

  if (argc == 2) {
    DumpSegmentsInFile(file.ptr());
  } else {
    const char* segment_name = argv[2];
    std::string_view data = FindSegmentInFile(file.ptr(), segment_name);
    if (!data.empty()) {
        if (strcmp(segment_name, LibMetadata_SYMBOL_NAME) == 0) {
          assert(data.size() == sizeof(LibMetadataT));
          printf("LibMetadata content:\n");
          auto* metadata = (const LibMetadataT*)(data.data());
          metadata->Print();
        } else {
          printf("%s content (size %u):\n", argv[2], (unsigned int)data.size());
          printf("  %s\n", data.data());
        }
      }
  }

  return 0;
}
