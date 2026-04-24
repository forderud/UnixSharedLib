#include <stdio.h>
#include <stdexcept>
#include <string.h>
#include <mach-o/loader.h>
#include <mach-o/swap.h>
#include "../FileMap.hpp"
#include "../LibMetadata.hpp"


void ParseSections(const char* file_ptr, const char* sect_ptr, uint32_t nsects) {
  for (uint32_t i = 0; i < nsects; ++i) {
    auto* sect = (const section_64*)sect_ptr;
    sect_ptr += sizeof(section_64);

    printf("    section: name=%s, offset=0x%x, size=0x%llx\n", sect->sectname, sect->offset, sect->size);
    printf("    data: %s\n", file_ptr + sect->offset);
  }
}

void DumpSegments(const FileMap& file) {
  auto& hdr = *(const mach_header_64*)file.ptr();

  const char* cmd_ptr = file.ptr() + sizeof(hdr);
  for (uint32_t i = 0; i < hdr.ncmds; ++i) {
    const auto* cmd = (const load_command*)cmd_ptr;
    cmd_ptr += cmd->cmdsize;
    //printf("Load command %u: cmd=0x%x, cmdsize=%u\n", i, cmd->cmd, cmd->cmdsize);

    // only parse 64bit segment commands
    if (cmd->cmd == LC_SEGMENT_64) {
      const auto* seg = (const segment_command_64*)cmd;
      printf("Segment command: segname=%s, vmaddr=0x%llx, vmsize=0x%llx, fileoff=0x%llx, filesize=0x%llx, nsects=%u\n",
             seg->segname, seg->vmaddr, seg->vmsize, seg->fileoff, seg->filesize, seg->nsects);
      ParseSections(file.ptr(), (const char*)seg + sizeof(segment_command_64), seg->nsects);
    }
  }
}

void FindSegmentInSections(const char* file_ptr, const char* sect_ptr, uint32_t nsects, const char* segment_name) {
  for (uint32_t i = 0; i < nsects; ++i) {
    auto* sect = (const section_64*)sect_ptr;
    sect_ptr += sizeof(section_64);

    if (strcmp(sect->sectname, segment_name) == 0) {
      printf("Found segment: %s:\n", segment_name);
      if (strcmp(segment_name, LibMetadata_SYMBOL_NAME) == 0) {
        auto* metadata = (const LibMetadataT*)(file_ptr + sect->offset);
        metadata->Print();
      } else {
        printf("%s\n", file_ptr + sect->offset);
      }
      return;
    }
  }
}

void FindSegmentInSegments(const FileMap& file, const char* segment_name) {
  auto& hdr = *(const mach_header_64*)file.ptr();

  const char* cmd_ptr = file.ptr() + sizeof(hdr);
  for (uint32_t i = 0; i < hdr.ncmds; ++i) {
    const auto* cmd = (const load_command*)cmd_ptr;
    cmd_ptr += cmd->cmdsize;

    // only parse 64bit segment commands
    if (cmd->cmd == LC_SEGMENT_64) {
      const auto* seg = (const segment_command_64*)cmd;
      FindSegmentInSections(file.ptr(), (const char*)seg + sizeof(segment_command_64), seg->nsects, segment_name);
    }
  }
}

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
