// based on https://github.com/eklitzke/parse-elf
#include <unistd.h>
#include <sys/mman.h>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>
#include "ElfUtils.hpp"


void PrintSymbolTable(const char *file_ptr, size_t str_off, size_t sym_off, size_t sym_sz) {
  // iterate over symbol table entries
  for (size_t i = 0; i*sizeof(Elf64_Sym) < sym_sz; i++) {
    Elf64_Sym sym{};
    memcpy(&sym, file_ptr + sym_off + i*sizeof(sym), sizeof(sym));

    auto binding = ELF64_ST_BIND(sym.st_info); // STB_LOCAL=0, STB_GLOBAL=1
    auto type = ELF64_ST_TYPE(sym.st_info); // STT_NOTYPE=0

    printf("SYMBOL TABLE ENTRY %zd:\n", i);
    printf("  st_name = %d", sym.st_name);
    if (sym.st_name != 0) {
      printf(" (%s)", file_ptr + str_off + sym.st_name);
    }
    printf("\n");
    printf("  st_info = binding %d, type %d\n", binding, type);
    printf("  st_other = %d\n", sym.st_other);
    printf("  st_shndx = %d\n", sym.st_shndx); // associated section header table index
    printf("  st_value = %p\n", (void *)sym.st_value);
    printf("  st_size = %zd\n", sym.st_size);
    printf("\n");
  }
  printf("\n");
}

void PrintSectionHeaders(const char *file_ptr) {
  Elf64_Ehdr elf_hdr{};
  memcpy(&elf_hdr, file_ptr, sizeof(elf_hdr));

  // section header string table
  ElfSectionHeader shst(file_ptr, elf_hdr.e_shstrndx);

  // parse Section header (Shdr)
  for (uint16_t i = 0; i < elf_hdr.e_shnum; i++) {
    ElfSectionHeader shdr(file_ptr, i);

    switch (shdr.sh_type) {
      case SHT_SYMTAB:
        {
          printf("found SHT_SYMTAB (symbol) table, sh_link %u (index %d)\n", shdr.sh_link, i);
          // get corresponding string table entry
          ElfSectionHeader st_shdr(file_ptr, shdr.sh_link);
          // print symbols
          PrintSymbolTable(file_ptr, st_shdr.sh_offset, shdr.sh_offset, shdr.sh_size);
        }
        break;
      case SHT_STRTAB:
        printf("found SHT_STRTAB (string) table, size %zd (index %d)\n", shdr.sh_size, i);
        break;
      case SHT_DYNSYM:
        printf("found SHT_DYNSYM table, size %zd (index %d)\n", shdr.sh_size, i);
        break;
      case SHT_PROGBITS:
        {
          printf("found SHT_PROGBITS (program data) table, size %zd (index %d)\n", shdr.sh_size, i);
        }
        break;
      default:
        printf("found %d table, size %zd (index %d)\n", shdr.sh_type, shdr.sh_size, i);
        break;
    }

    printf("  sh_addr 0x%lx\n", shdr.sh_addr);
    printf("  sh_offset 0x%lx\n", shdr.sh_offset);
    printf("  name: %s\n", file_ptr + shst.sh_offset + shdr.sh_name);
  }
}

std::string_view FindDataInSymbolTable(const char *file_ptr, size_t str_off, size_t sym_off, size_t sym_sz, const char* symbol_name_prefix) {
  uint16_t st_shndx = 0;
  size_t start_offset = (size_t)-1;
  size_t end_offset = (size_t)-1;

  // iterate over symbol table entries
  for (size_t i = 0; i*sizeof(Elf64_Sym) < sym_sz; i++) {
    Elf64_Sym sym{};
    memcpy(&sym, file_ptr + sym_off + i*sizeof(sym), sizeof(sym));

    size_t sym_len = strlen(symbol_name_prefix);
    
    const char* cur_name = file_ptr + str_off + sym.st_name;
    size_t cur_len = strlen(cur_name);

    if ((strncmp(cur_name, symbol_name_prefix, sym_len) == 0) && (cur_len > sym_len)) {
      if (strcmp(cur_name + sym_len, "_start") == 0) {
        printf("found symbol %s START at index %zd\n", symbol_name_prefix, i);
        st_shndx = sym.st_shndx;
        start_offset = sym.st_value;
      } else if (strcmp(cur_name + sym_len, "_end") == 0) {
        printf("found symbol %s END at index %zd\n", symbol_name_prefix, i);
        end_offset = sym.st_value;
      }
    }
  }

  if ((start_offset == (size_t)-1) || (end_offset == (size_t)-1))
    return {}; // start/end symbols not found

  // determine data section address range
  ElfSectionHeader data(file_ptr, st_shndx);
  const char* start = file_ptr + start_offset + data.sh_offset - data.sh_addr;
  const char* end = file_ptr + end_offset + data.sh_offset - data.sh_addr;
  return std::string_view(start, end - start);
}


std::string_view FindDataSection(const char *file_ptr, const char* symbol_name_prefix) {
  // recipie for extracting embedded symbols:
  // 1: Find SHT_SYMTAB section "st" with the desired symbol
  // 2: Find corresponding .data section "data" from st_shndx
  // 3: Content: file.ptr() + st.st_value + data.sh_offset - data.sh_addr
  Elf64_Ehdr elf_hdr{};
  memcpy(&elf_hdr, file_ptr, sizeof(elf_hdr));

  // section header string table
  ElfSectionHeader shst(file_ptr, elf_hdr.e_shstrndx);

  // iterate over Section header list (Shdr)
  for (uint16_t i = 0; i < elf_hdr.e_shnum; i++) {
    ElfSectionHeader shdr(file_ptr, i);

    if (shdr.sh_type != SHT_SYMTAB)
      continue;

    // get corresponding string table entry
    ElfSectionHeader st_shdr(file_ptr, shdr.sh_link);
    // get symbol data
    std::string_view data = FindDataInSymbolTable(file_ptr, st_shdr.sh_offset, shdr.sh_offset, shdr.sh_size, symbol_name_prefix);
    if (!data.empty())
      return data;
  }

  return {};
}

int main(int argc, char **argv) {
  if (argc < 2) {
    printf("usage: %s <elf-binary>\n", argv[0]);
    return 1;
  }

  printf("Parsing ELF symbols in %s...\n", argv[1]);
  FileMap file(argv[1]);

  Elf64_Ehdr elf_hdr{};
  memcpy(&elf_hdr, file.ptr(), sizeof(elf_hdr));

  const unsigned char expected_magic[] = {ELFMAG0, ELFMAG1, ELFMAG2, ELFMAG3};
  if (memcmp(elf_hdr.e_ident, expected_magic, sizeof(expected_magic)) != 0) {
    std::cerr << "Target is not an ELF executable\n";
    return 1;
  }
  if (elf_hdr.e_ident[EI_CLASS] != ELFCLASS64) {
    std::cerr << "Sorry, only ELF-64 is supported.\n";
    return 1;
  }

  if (argc == 2) {
    PrintSectionHeaders(file.ptr());
  } else {
    std::string_view data = FindDataSection(file.ptr(), argv[2]);
    printf("%s content (size %u):\n", argv[2], data.size());
    printf("%.*s\n", (int)data.size(), data.data());
  }

  printf("\n");
  return 0;
}
