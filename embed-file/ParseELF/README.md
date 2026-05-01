## ELF embed and parsing samples

#### Demonstrates the following
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
Symbol table of section table .dynsym (Index 3) with 15 entries
-----------------------
Nr             Value               Size                Info (Binding|Type) Other          Shndx          Name           
...
9              0x000000000002005C  0x0000000000000000  GLOBAL | NOTYPE     DEFAULT        0x0016         _binary_embed_example_txt_end
10             0x0000000000020033  0x0000000000000000  GLOBAL | NOTYPE     DEFAULT        0x0016         _binary_embed_example_txt_start
...
13             0x0000000000000029  0x0000000000000000  GLOBAL | NOTYPE     DEFAULT        0xFFF1         _binary_embed_example_txt_size
...
Symbol table of section table .symtab (Index 26) with 82 entries
-----------------------
   Num:    Value          Size Type    Bind   Vis      Ndx Name
...
72             0x0000000000020033  0x0000000000000000  GLOBAL | NOTYPE     DEFAULT        0x0016         _binary_embed_example_txt_start
...
77             0x0000000000000029  0x0000000000000000  GLOBAL | NOTYPE     DEFAULT        0xFFF1         _binary_embed_example_txt_size
...
79             0x000000000002005C  0x0000000000000000  GLOBAL | NOTYPE     DEFAULT        0x0016         _binary_embed_example_txt_end
...
```

The `.dynsym` symbols only appear if linking the binary file into a shared library (`.so`). It does _not_ appear if linking directly into an executable.

## Documentation
* Linux manual page - [elf(5)](https://man7.org/linux/man-pages/man5/elf.5.html) - documents `Elf64_Ehdr`, `Elf64_Shdr`, `Elf64_Sym`
* GNU binutils [LD](https://sourceware.org/binutils/docs/ld.html) doc
* Sunshine's [Online ELF Viewer](https://www.sunshine2k.de/coding/javascript/onlineelfviewer/onlineelfviewer.html)

