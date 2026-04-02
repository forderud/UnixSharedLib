#include <stdio.h>
#include <stdexcept>
#include <unistd.h> // open/close
#include <sys/mman.h> // mmap/munmap
#include <mach-o/loader.h>
#include <mach-o/swap.h>
#include <fcntl.h> // open/close
#include <sys/stat.h> // fstat

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

int main(int argc, char **argv) {
  if (argc < 2) {
    printf("usage: %s <macho-binary>\n", argv[0]);
    return 1;
  }

  printf("Parsing Mach-O symbols in %s...\n", argv[1]);
  FileMap file(argv[1]);

#if 0
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
#endif

  printf("\n");
  return 0;
}
