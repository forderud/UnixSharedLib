Reproducer for problem with `-static-libstdc++` in shared libraries by Aleksei Skorodumov.

**GOAL**: Develop a Linux shared library (.so) with as few run-time dependencies as possible, so that it becomes compatible with multiple distros of various age.

**PROBLEM**: Segfault if combining `-static-libstdc++` with `-Wl,--exclude-libs,ALL` in shared library. Unsure if this is due to a bug or inherent limitation in libstdc++.

**WARNING**: According to *Safe to link to static libstdc++ from shared library?* [response 1](https://gcc.gnu.org/pipermail/libstdc++/2025-November/064342.html) and [response 2](https://gcc.gnu.org/pipermail/libstdc++/2025-November/064347.html) then it's _not_ possible to fully encapsulate libstdc++ usage inside a shared library.

## External resources
Documentation:
* GCC [Options for Linking](https://gcc.gnu.org/onlinedocs/gcc/Link-Options.html) - documents `-static-libstdc++` (doc source: [gcc/doc/invoke.texi](https://github.com/gcc-mirror/gcc/blob/master/gcc/doc/invoke.texi#L20038))
* GNU binutils [LD](https://sourceware.org/binutils/docs/ld.html) - documents `--exclude-libs`
