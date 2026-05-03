#include "ParseMach.hpp"


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
    DumpSegments(file);
  } else {
    const char* segment_name = argv[2];
    FindSegmentInSegments(file, segment_name);
  }

  printf("\n");
  return 0;
}
