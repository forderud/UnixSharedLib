#include <stdio.h>
#include <mach-o/loader.h>
#include <mach-o/swap.h>


int main(int argc, char **argv) {
  if (argc < 2) {
    printf("usage: %s <macho-binary>\n", argv[0]);
    return 1;
  }

  printf("Parsing Mach-O symbols in %s...\n", argv[1]);
#if 0
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
#endif

  printf("\n");
  return 0;
}
