#include <stdio.h>
#include <stdexcept>
#include <string.h>
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

void ParseSections(const char* ptr, uint32_t nsects) {
  if(nsects == 0) {
    return;
  }

  auto* sect = (const section_64*)ptr;
  printf("    section: name %s\n", sect->sectname);
}

int main(int argc, char **argv) {
  if (argc < 2) {
    printf("usage: %s <macho-binary>\n", argv[0]);
    return 1;
  }

  printf("Parsing Mach-O symbols in %s...\n", argv[1]);
  FileMap file(argv[1]);

  mach_header_64 hdr{};
  memcpy(&hdr, file.ptr(), sizeof(hdr));

  if (hdr.magic != MH_MAGIC_64) {
    printf("Target is not a valid Mach-O 64-bit executable\n");
    return 1;
  }

  printf("Mach-O header: cputype=0x%x, cpusubtype=0x%x, filetype=0x%x, ncmds=%u, sizeofcmds=%u\n",
         hdr.cputype, hdr.cpusubtype, hdr.filetype, hdr.ncmds, hdr.sizeofcmds);
  printf("\n");

  const char* cmd_ptr = file.ptr() + sizeof(hdr);
  for (uint32_t i = 0; i < hdr.ncmds; ++i) {
    const auto* cmd = (const load_command*)cmd_ptr;
    cmd_ptr += cmd->cmdsize;
    printf("Command %u: cmd=0x%x, cmdsize=%u\n", i, cmd->cmd, cmd->cmdsize);

    if (cmd->cmd == LC_SEGMENT_64) {
      const auto* seg = (const segment_command_64*)cmd;
      printf("  Segment command: segname=%s, vmaddr=0x%llx, vmsize=0x%llx, fileoff=0x%llx, filesize=0x%llx, nsects=%u\n",
             seg->segname, seg->vmaddr, seg->vmsize, seg->fileoff, seg->filesize, seg->nsects);
      ParseSections((const char*)seg + sizeof(segment_command_64), seg->nsects);
    }

  }

  printf("\n");
  return 0;
}
