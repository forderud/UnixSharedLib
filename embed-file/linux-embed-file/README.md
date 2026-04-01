## ELF embed and parsing samples

#### Goals
* Embed arbitrary binary files in shared libraries
* Access the embedded files without running the binary

#### Challenges
Files embeddded with `ld -r -b binary embed_example.txt -z noexecstack -o embed_example.o` or `objcopy --input-target binary --output elf64-x86-64 --add-section .note.GNU-stack=/dev/null embed_example.txt embed_example.o` aren't exported by the library.

## readelf symbol dump (extract of output)
```
embed-file/linux-embed-file$ readelf -s mainApp 
...
Symbol table '.symtab' contains 99 entries:
   Num:    Value          Size Type    Bind   Vis      Ndx Name
...
    78: 000000000042002c     0 NOTYPE  GLOBAL DEFAULT   23 _binary_embed_ex[...]
...
    93: 000000000000000c     0 NOTYPE  GLOBAL DEFAULT  ABS _binary_embed_ex[...]
...
    95: 0000000000420038     0 NOTYPE  GLOBAL DEFAULT   23 _binary_embed_ex[...]
...
```

## Documentation
* Linux manual page - [elf(5)](https://man7.org/linux/man-pages/man5/elf.5.html) - documents `Elf64_Ehdr`, `Elf64_Phdr`, `Elf64_Shdr`, `Elf64_Sym`
* GNU binutils [LD](https://sourceware.org/binutils/docs/ld.html) doc
