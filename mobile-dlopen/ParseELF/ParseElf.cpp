// based on https://github.com/eklitzke/parse-elf
#include <unistd.h>
#include <cassert>
#include "ParseElf.hpp"
#include "../FileMap.hpp"


int main(int argc, char **argv) {
  if (argc < 2) {
    printf("usage: %s <elf-binary>\n", argv[0]);
    return 1;
  }

#if 0
  printf("Parsing ELF symbols in %s...\n", argv[1]);
#endif
  FileMap file(argv[1]);

  const auto& elf_hdr = *(const Elf64_Ehdr*)file.ptr();

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
    PrintSectionHeadersInFile(file.ptr());
  } else {
    std::string_view data = FindDataSectionInFile(file.ptr(), argv[2]);
    if (data.size() > 0) {
      if (strcmp(argv[2], LibMetadata_SYMBOL_NAME) == 0) {
        auto* metadata = (const LibMetadataT*)data.data();
        metadata->Print();
      } else {
        printf("%s content (size %u):\n", argv[2], (unsigned int)data.size());
        printf("%.*s\n", (int)data.size(), data.data());
      }
    } else {
      printf("ERROR: Unable to find symbol %s\n", argv[2]);
    }
  }

  printf("\n");
  return 0;
}
