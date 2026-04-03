#pragma once
#if defined(__APPLE__)
  #include <unistd.h> // open/close
  #include <sys/mman.h> // mmap/munmap
#else
  #include <fcntl.h> // open/close
  #include <sys/stat.h> // fstat
#endif


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
