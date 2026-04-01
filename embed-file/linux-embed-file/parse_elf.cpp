// based on https://github.com/eklitzke/parse-elf
#include <elf.h>
#include <fcntl.h> // open/close
#include <sys/stat.h> // fstat
#include <unistd.h>
#include <sys/mman.h>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <iostream>


void PrintSymbolTable(const char *file_ptr, size_t str_off, size_t sym_off, size_t sym_sz) {
  //printf("str_off = %zd\n", str_off);
  //printf("sym_off = %zd\n", sym_off);
  //printf("sym_sz = %zd\n", sym_sz);

  for (size_t j = 0; j * sizeof(Elf64_Sym) < sym_sz; j++) {
    Elf64_Sym sym{};
    memcpy(&sym, file_ptr + sym_off + j*sizeof(Elf64_Sym), sizeof(sym));

    if (!sym.st_name)
      continue; // skip entries without name

    auto binding = ELF64_ST_BIND(sym.st_info); // STB_LOCAL=0, STB_GLOBAL=1
    auto type = ELF64_ST_TYPE(sym.st_info); // STT_NOTYPE=0

    printf("SYMBOL TABLE ENTRY %zd\n", j);
    printf("st_name = %d", sym.st_name);
    if (sym.st_name != 0) {
      printf(" (%s)", file_ptr + str_off + sym.st_name);
    }
    printf("\n");
    printf("st_info = binding %d, type %d\n", binding, type);
    printf("st_other = %d\n", sym.st_other);
    printf("st_shndx = %d\n", sym.st_shndx); // associated section header table index
    printf("st_value = %p\n", (void *)sym.st_value);
    printf("st_size = %zd\n", sym.st_size);
    printf("\n");
  }
  printf("\n");
}

/** Memory-map a file into the process address space. */
class FileMap {
public:
    FileMap(const char* filename) {
        int fd = open(filename, O_RDONLY);
        if (fd == -1) {
            throw std::runtime_error("file open failed");
        }

        struct stat sb{};
        if (fstat(fd, &sb) == -1) { // obtain file size
            close(fd);
            throw std::runtime_error("unable to determine file size");
        }
        m_size = sb.st_size;

        m_ptr = (const char*)mmap(nullptr, m_size, PROT_READ, MAP_PRIVATE, fd, 0);
        if (m_ptr == nullptr) {
            close(fd);
            throw std::runtime_error("unable to map file");
        }
        close(fd);
    }
    ~FileMap() {
        munmap((void*)m_ptr, m_size);
    }
    
public:
    size_t      m_size = 0;
    const char* m_ptr = nullptr;
};

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("usage: %s <elf-binary>\n", argv[0]);
    return 1;
  }

  FileMap file(argv[1]);

  Elf64_Ehdr elf_hdr{};
  memcpy(&elf_hdr, file.m_ptr, sizeof(elf_hdr));

  const unsigned char expected_magic[] = {ELFMAG0, ELFMAG1, ELFMAG2, ELFMAG3};
  if (memcmp(elf_hdr.e_ident, expected_magic, sizeof(expected_magic)) != 0) {
    std::cerr << "Target is not an ELF executable\n";
    return 1;
  }
  if (elf_hdr.e_ident[EI_CLASS] != ELFCLASS64) {
    std::cerr << "Sorry, only ELF-64 is supported.\n";
    return 1;
  }

  {
    printf("file size: %zd\n", file.m_size);
    printf("program header offset: %zd\n", elf_hdr.e_phoff);
    printf("program header num: %d\n", elf_hdr.e_phnum);
    printf("section header offset: %zd\n", elf_hdr.e_shoff);
    printf("section header num: %d\n", elf_hdr.e_shnum);
    printf("section header string table: %d\n", elf_hdr.e_shstrndx);

    size_t string_offset = elf_hdr.e_shstrndx;
    printf("string offset at %zd\n", string_offset);
    printf("\n");
  }

#if 0
  for (uint16_t i = 0; i < elf_hdr.e_phnum; i++) {
    size_t offset = elf_hdr.e_phoff + i * elf_hdr.e_phentsize;
    Elf64_Phdr phdr{};
    memcpy(&phdr, file.m_ptr + offset, sizeof(phdr));
 
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
#endif

  // parse Section header (Shdr)
  for (uint16_t i = 0; i < elf_hdr.e_shnum; i++) {
    size_t offset = elf_hdr.e_shoff + i * elf_hdr.e_shentsize;
    Elf64_Shdr shdr{};
    memcpy(&shdr, file.m_ptr + offset, sizeof(shdr));

    switch (shdr.sh_type) {
      case SHT_SYMTAB:
        {
          printf("found symtab table at offset %zd, sh_link %u (index %d)\n", shdr.sh_offset, shdr.sh_link, i);
          // get corresponding string table entry
          size_t st_offset = elf_hdr.e_shoff + shdr.sh_link * elf_hdr.e_shentsize;
          Elf64_Shdr st_shdr{};
          memcpy(&st_shdr, file.m_ptr + st_offset, sizeof(st_shdr));
          // print symbols
          PrintSymbolTable(file.m_ptr, st_shdr.sh_offset, shdr.sh_offset, shdr.sh_size);
        }
        break;
      case SHT_STRTAB:
        printf("found string table at offset %zd (index %d)\n", shdr.sh_offset, i);
        // TODO: have to handle multiple string tables better
        //if (!dynstr_off) {
        //  dynstr_off = shdr.sh_offset;
        //}
        break;
      case SHT_DYNSYM:
        //dynsym_off = shdr.sh_offset;
        //dynsym_sz = shdr.sh_size;
        //printf("found dynsym table at offset %zd, size %zd (index %d)\n", shdr.sh_offset, shdr.sh_size, i);
        break;
      case SHT_PROGBITS:
        printf("found PROGBITS table at offset %zd, size %zd (index %d)\n", shdr.sh_offset, shdr.sh_size, i);
        break;
      default:
        //printf("found %d table at offset %zd, size %zd (index %d)\n", shdr.sh_type, shdr.sh_offset, shdr.sh_size, i);
        break;
    }
  }
  printf("\n");
  return 0;
}
