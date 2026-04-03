#pragma once
#include <elf.h>


struct ElfSectionHeader : Elf64_Shdr {
    ElfSectionHeader(const char* file_ptr, uint16_t idx) {
        Elf64_Ehdr elf_hdr{};
        memcpy(&elf_hdr, file_ptr, sizeof(elf_hdr));
        // populate struct
        memcpy(this, file_ptr + elf_hdr.e_shoff + idx*elf_hdr.e_shentsize, sizeof(Elf64_Shdr));
    }
};
static_assert(sizeof(ElfSectionHeader) == sizeof(Elf64_Shdr), "ElfSectionHeader size mismatch");
