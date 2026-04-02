## ELF embed and parsing samples

#### Demonstrates the folllowing
* Embed arbitrary binary files in shared libraries
* Access the embedded files without running the binary

## File embedding commands
With `ld`:
```
ld -r -b binary embed_example.txt -z noexecstack -o embed_example.o
```

... or with `objcopy`:
```
if [[ $(uname -m) == "aarch64" ]]; then
    bfdname="elf64-littleaarch64"
else
    bfdname="elf64-x86-64"
fi
objcopy --input-target binary --output $bfdname --add-section .note.GNU-stack=/dev/null embed_example.txt embed_example.o
```

## readelf symbol dump (extract of output)
```
embed-file/linux-embed-file$ readelf -s libmylib.so 
...
Symbol table '.symtab' contains 82 entries:
   Num:    Value          Size Type    Bind   Vis      Ndx Name
...
    72: 0000000000020033     0 NOTYPE  GLOBAL DEFAULT   22 _binary_embed_ex[...]
...
    77: 0000000000000029     0 NOTYPE  GLOBAL DEFAULT  ABS _binary_embed_ex[...]
...
    79: 000000000002005c     0 NOTYPE  GLOBAL DEFAULT   22 _binary_embed_ex[...]
...
```

## Documentation
* Linux manual page - [elf(5)](https://man7.org/linux/man-pages/man5/elf.5.html) - documents `Elf64_Ehdr`, `Elf64_Phdr`, `Elf64_Shdr`, `Elf64_Sym`
* GNU binutils [LD](https://sourceware.org/binutils/docs/ld.html) doc
