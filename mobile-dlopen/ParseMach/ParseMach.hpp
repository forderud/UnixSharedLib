#pragma once
#include <stdio.h>
#include <string_view>
#include <mach-o/loader.h>


void ParseSections(const char* file_ptr, const char* sect_ptr, uint32_t nsects) {
  for (uint32_t i = 0; i < nsects; ++i) {
    auto* sect = (const section_64*)sect_ptr;
    sect_ptr += sizeof(section_64);

    printf("    section: name=%s, offset=0x%x, size=0x%llx\n", sect->sectname, sect->offset, sect->size);
    printf("    data: %s\n", file_ptr + sect->offset);
  }
}

void DumpSegmentsInFile(const char* file_ptr) {
  auto& hdr = *(const mach_header_64*)file_ptr;

  const char* cmd_ptr = file_ptr + sizeof(hdr);
  for (uint32_t i = 0; i < hdr.ncmds; ++i) {
    const auto* cmd = (const load_command*)cmd_ptr;
    cmd_ptr += cmd->cmdsize;
    //printf("Load command %u: cmd=0x%x, cmdsize=%u\n", i, cmd->cmd, cmd->cmdsize);

    // only parse 64bit segment commands
    if (cmd->cmd == LC_SEGMENT_64) {
      const auto* seg = (const segment_command_64*)cmd;
      printf("Segment command: segname=%s, vmaddr=0x%llx, vmsize=0x%llx, fileoff=0x%llx, filesize=0x%llx, nsects=%u\n",
             seg->segname, seg->vmaddr, seg->vmsize, seg->fileoff, seg->filesize, seg->nsects);
      ParseSections(file_ptr, (const char*)seg + sizeof(segment_command_64), seg->nsects);
    }
  }
}

std::string_view FindSegmentInSections(const char* file_ptr, const char* sect_ptr, uint32_t nsects, const char* segment_name) {
  for (uint32_t i = 0; i < nsects; ++i) {
    auto* sect = (const section_64*)sect_ptr;
    sect_ptr += sizeof(section_64);

    if (strcmp(sect->sectname, segment_name) == 0)
      return std::string_view(file_ptr + sect->offset, sect->size);
  }

  return std::string_view(); // not found
}

std::string_view FindSegmentInFile(const char* file_ptr, const char* segment_name) {
  auto& hdr = *(const mach_header_64*)file_ptr;

  const char* cmd_ptr = file_ptr + sizeof(hdr);
  for (uint32_t i = 0; i < hdr.ncmds; ++i) {
    const auto* cmd = (const load_command*)cmd_ptr;
    cmd_ptr += cmd->cmdsize;

    // only parse 64bit segment commands
    if (cmd->cmd == LC_SEGMENT_64) {
      const auto* seg = (const segment_command_64*)cmd;
      std::string_view data = FindSegmentInSections(file_ptr, (const char*)seg + sizeof(segment_command_64), seg->nsects, segment_name);
      if (!data.empty())
        return data;
    }
  }
  return std::string_view(); // not found
}
