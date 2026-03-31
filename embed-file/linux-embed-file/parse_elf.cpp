// based on https://github.com/eklitzke/parse-elf
#include <elf.h>
#include <unistd.h>
#include <sys/mman.h>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <iostream>


void PrintSymbolTable(char *cbytes, size_t str_off, size_t sym_off, size_t sym_sz) {
  printf("str_off = %zd\n", str_off);
  printf("sym_off = %zd\n", sym_off);
  printf("sym_sz = %zd\n", sym_sz);

  for (size_t j = 0; j * sizeof(Elf64_Sym) < sym_sz; j++) {
    Elf64_Sym sym;
    size_t absoffset = sym_off + j * sizeof(Elf64_Sym);
    memcpy(&sym, cbytes + absoffset, sizeof(sym));

    printf("SYMBOL TABLE ENTRY %zd\n", j);
    printf("st_name = %d", sym.st_name);
    if (sym.st_name != 0) {
      printf(" (%s)", cbytes + str_off + sym.st_name);
    }
    printf("\n");
    printf("st_info = %d\n", sym.st_info);
    printf("st_other = %d\n", sym.st_other);
    printf("st_shndx = %d\n", sym.st_shndx);
    printf("st_value = %p\n", (void *)sym.st_value);
    printf("st_size = %zd\n", sym.st_size);
    printf("\n");
  }
  printf("\n");
}


int main(int argc, char **argv) {
  if (argc != 2) {
    printf("usage: %s <elf-binary>\n", argv[0]);
    return 1;
  }
  FILE *pyfile = fopen(argv[1], "r");
  if (pyfile == nullptr) {
    return 1;
  }
  if (fseek(pyfile, 0, SEEK_END)) {
    fclose(pyfile);
    return 1;
  }
  long pyfile_size = ftell(pyfile);

  void *pybytes = mmap(nullptr, (size_t)pyfile_size, PROT_READ, MAP_PRIVATE,
                       fileno(pyfile), 0);
  if (pybytes == nullptr) {
    fclose(pyfile);
    perror("mmap()");
    return 1;
  }
  fclose(pyfile);
  printf("%p\n", pybytes);

  Elf64_Ehdr elf_hdr;
  memcpy(&elf_hdr, pybytes, sizeof(elf_hdr));

  const unsigned char expected_magic[] = {ELFMAG0, ELFMAG1, ELFMAG2, ELFMAG3};
  if (memcmp(elf_hdr.e_ident, expected_magic, sizeof(expected_magic)) != 0) {
    std::cerr << "Target is not an ELF executable\n";
    return 1;
  }
  if (elf_hdr.e_ident[EI_CLASS] != ELFCLASS64) {
    std::cerr << "Sorry, only ELF-64 is supported.\n";
    return 1;
  }
#if 0
  if (elf_hdr.e_ident[EI_OSABI] != ELFOSABI_LINUX) {
    std::cerr << "Sorry, only ELFOSABI_LINUX is supported.\n";
    return 1;
  }
#endif

  printf("file size: %zd\n", pyfile_size);
  printf("program header offset: %zd\n", elf_hdr.e_phoff);
  printf("program header num: %d\n", elf_hdr.e_phnum);
  printf("section header offset: %zd\n", elf_hdr.e_shoff);
  printf("section header num: %d\n", elf_hdr.e_shnum);
  printf("section header string table: %d\n", elf_hdr.e_shstrndx);

  size_t string_offset = elf_hdr.e_shstrndx;
  printf("string offset at %zd\n", string_offset);
  printf("\n");

  char *cbytes = (char *)pybytes;
  for (uint16_t i = 0; i < elf_hdr.e_phnum; i++) {
    size_t offset = elf_hdr.e_phoff + i * elf_hdr.e_phentsize;
    Elf64_Phdr phdr;
    memcpy(&phdr, pybytes + offset, sizeof(phdr));
 
    printf("PROGRAM HEADER %d, offset = %zd\n", i, offset);
    printf("========================\n");
    printf("p_type = ");
    switch (phdr.p_type) {
      case PT_NULL:
        puts("PT_NULL");
        break;
      case PT_LOAD:
        puts("PT_LOAD");
        break;
      case PT_DYNAMIC:
        puts("PT_DYNAMIC");
        break;
      case PT_INTERP:
        puts("PT_INTERP");
        break;
      case PT_NOTE:
        puts("PT_NOTE");
        break;
      case PT_SHLIB:
        puts("PT_SHLIB");
        break;
      case PT_PHDR:
        puts("PT_PHDR");
        break;
      case PT_LOPROC:
        puts("PT_LOPROC");
        break;
      case PT_HIPROC:
        puts("PT_HIPROC");
        break;
      case PT_GNU_STACK:
        puts("PT_GNU_STACK");
        break;
      default:
        printf("UNKNOWN/%d\n", phdr.p_type);
        break;
    }
    printf("p_offset = %zd\n", phdr.p_offset);
    printf("p_vaddr = %zd\n", phdr.p_vaddr);
    printf("p_paddr = %zd\n", phdr.p_paddr);
    printf("p_filesz = %zd\n", phdr.p_filesz);
    printf("p_memsz = %zd\n", phdr.p_memsz);
    printf("p_flags = %d\n", phdr.p_flags);
    printf("p_align = %lu\n", phdr.p_align);
    printf("\n");
  }

  size_t dynstr_off = 0;
  size_t dynsym_off = 0;
  size_t dynsym_sz = 0;

  // parse Section header (Shdr)
  for (uint16_t i = 0; i < elf_hdr.e_shnum; i++) {
    size_t offset = elf_hdr.e_shoff + i * elf_hdr.e_shentsize;
    Elf64_Shdr shdr;
    memcpy(&shdr, pybytes + offset, sizeof(shdr));

    switch (shdr.sh_type) {
      case SHT_SYMTAB:
        printf("found symtab table at offset %zd (index %zd)\n", shdr.sh_offset, i);
        // TODO: implement
        break;
      case SHT_STRTAB:
        printf("found string table at offset %zd (index %zd)\n", shdr.sh_offset, i);
        // TODO: have to handle multiple string tables better
        if (!dynstr_off) {
          dynstr_off = shdr.sh_offset;
        }
        break;
      case SHT_DYNSYM:
        dynsym_off = shdr.sh_offset;
        dynsym_sz = shdr.sh_size;
        printf("found dynsym table at %zd, size %zd (index %zd)\n", shdr.sh_offset, shdr.sh_size, i);
        break;
      default:
        break;
    }
  }
  printf("\n");

  assert(dynstr_off);
  assert(dynsym_off);
  PrintSymbolTable(cbytes, dynstr_off, dynsym_off, dynsym_sz);
  
  return 0;
}
