#pragma once
#include <elf.h>


struct ElfSectionHeader : Elf64_Shdr {
    ElfSectionHeader(const char* file_ptr, uint16_t idx) {
        const auto& elf_hdr = *(const Elf64_Ehdr*)file_ptr;
        // populate struct
        memcpy(this, file_ptr + elf_hdr.e_shoff + idx*elf_hdr.e_shentsize, sizeof(Elf64_Shdr));
    }
};
static_assert(sizeof(ElfSectionHeader) == sizeof(Elf64_Shdr), "ElfSectionHeader size mismatch");


struct ElfSymbol : Elf64_Sym {
    ElfSymbol(const char* file_ptr, size_t sym_off, size_t idx) {
        const auto& sym = *(const Elf64_Sym*)(file_ptr + sym_off + idx*sizeof(Elf64_Sym));
        // populate struct
        memcpy(this, &sym, sizeof(Elf64_Sym));
    }
};
static_assert(sizeof(ElfSymbol) == sizeof(Elf64_Sym), "ElfSymbol size mismatch");
