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
#include <vector>


void PrintSymbolTable(const char *file_ptr, size_t str_off, size_t sym_off, size_t sym_sz) {
  //printf("str_off = %zd\n", str_off);
  //printf("sym_off = %zd\n", sym_off);
  //printf("sym_sz = %zd\n", sym_sz);

  for (size_t j = 0; j * sizeof(Elf64_Sym) < sym_sz; j++) {
    Elf64_Sym sym{};
    memcpy(&sym, file_ptr + sym_off + j*sizeof(Elf64_Sym), sizeof(sym));

    //if (!sym.st_name)
    //  continue; // skip entries without name

    auto binding = ELF64_ST_BIND(sym.st_info); // STB_LOCAL=0, STB_GLOBAL=1
    auto type = ELF64_ST_TYPE(sym.st_info); // STT_NOTYPE=0

    printf("SYMBOL TABLE ENTRY %zd:\n", j);
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
    
    size_t size() const {
        return m_size;
    }
    const char* ptr() const {
        return m_ptr;
    }
    
private:
    size_t      m_size = 0;
    const char* m_ptr = nullptr;
};

struct ElfSectionHeader : Elf64_Shdr {
    ElfSectionHeader(const char* file_ptr, uint16_t idx) {
        Elf64_Ehdr elf_hdr{};
        memcpy(&elf_hdr, file_ptr, sizeof(elf_hdr));
        // populate struct
        memcpy(this, file_ptr + elf_hdr.e_shoff + idx*elf_hdr.e_shentsize, sizeof(Elf64_Shdr));
    }
};
static_assert(sizeof(ElfSectionHeader) == sizeof(Elf64_Shdr), "ElfSectionHeader size mismatch");


int main(int argc, char **argv) {
  if (argc != 2) {
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

#if 0
  {
    printf("file size: %zd\n", file.size());
    printf("program header offset: %zd\n", elf_hdr.e_phoff);
    printf("program header num: %d\n", elf_hdr.e_phnum);
    printf("section header offset: %zd\n", elf_hdr.e_shoff);
    printf("section header num: %d\n", elf_hdr.e_shnum);
    printf("section header string table: %d\n", elf_hdr.e_shstrndx);
    printf("\n");
  }
#endif

#if 0
  for (uint16_t i = 0; i < elf_hdr.e_phnum; i++) {
    Elf64_Phdr phdr{};
    memcpy(&phdr, file.ptr() + elf_hdr.e_phoff + i*elf_hdr.e_phentsize, sizeof(phdr));
 
    printf("PROGRAM HEADER %d:\n", i);
    printf("  p_type = ");
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
    printf("  p_offset = %zd\n", phdr.p_offset);
    printf("  p_vaddr = %zd\n", phdr.p_vaddr);
    printf("  p_paddr = %zd\n", phdr.p_paddr);
    printf("  p_filesz = %zd\n", phdr.p_filesz);
    printf("  p_memsz = %zd\n", phdr.p_memsz);
    printf("  p_flags = %d\n", phdr.p_flags);
    printf("  p_align = %lu\n", phdr.p_align);
    printf("\n");
  }
#endif

  // parse Section header (Shdr)
  for (uint16_t i = 0; i < elf_hdr.e_shnum; i++) {
    ElfSectionHeader shdr(file.ptr(), i);

    switch (shdr.sh_type) {
      case SHT_SYMTAB:
        {
          printf("found SHT_SYMTAB (symbol) table, sh_link %u (index %d)\n", shdr.sh_link, i);
          // get corresponding string table entry
          ElfSectionHeader st_shdr(file.ptr(), shdr.sh_link);
          // print symbols
          PrintSymbolTable(file.ptr(), st_shdr.sh_offset, shdr.sh_offset, shdr.sh_size);
        }
        break;
      case SHT_STRTAB:
        printf("found SHT_STRTAB (string) table, size %zd (index %d)\n", shdr.sh_size, i);
        // TODO: have to handle multiple string tables better
        //if (!dynstr_off) {
        //  dynstr_off = shdr.sh_offset;
        //}
        break;
      case SHT_DYNSYM:
        //dynsym_off = shdr.sh_offset;
        //dynsym_sz = shdr.sh_size;
        printf("found SHT_DYNSYM table, size %zd (index %d)\n", shdr.sh_size, i);
        break;
      case SHT_PROGBITS:
        {
          printf("found SHT_PROGBITS (program data) table, size %zd (index %d)\n", shdr.sh_size, i);
          std::vector<char> buffer(shdr.sh_size);
          memcpy(buffer.data(), file.ptr() + shdr.sh_offset, buffer.size());
          for(char& c: buffer) {
              if (c == 0) c = ' '; // convert null termination to spaces to display of the entire buffer
          }
          printf("  content: %.*s\n", (int)buffer.size(), buffer.data());
        }
        break;
      default:
        printf("found %d table, size %zd (index %d)\n", shdr.sh_type, shdr.sh_size, i);
        break;
    }

    printf("  sh_offset 0x%lx\n", shdr.sh_offset);
    printf("  sh_name %u\n", shdr.sh_name); // TODO: Resolve string names
  }

  printf("\n");
  return 0;
}
