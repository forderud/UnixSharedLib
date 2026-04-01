#pragma once
#include <elf.h>
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

struct ElfSectionHeader : Elf64_Shdr {
    ElfSectionHeader(const char* file_ptr, uint16_t idx) {
        Elf64_Ehdr elf_hdr{};
        memcpy(&elf_hdr, file_ptr, sizeof(elf_hdr));
        // populate struct
        memcpy(this, file_ptr + elf_hdr.e_shoff + idx*elf_hdr.e_shentsize, sizeof(Elf64_Shdr));
    }
};
static_assert(sizeof(ElfSectionHeader) == sizeof(Elf64_Shdr), "ElfSectionHeader size mismatch");
