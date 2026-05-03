Experiments with static linking of the C++ standard library and other dependencies in Linux shared libraries.

**PROBLEM**: How to encapsulate C++ usage in a shared library (`.so`) with a "C" API without exposing any C++ symbols to the client?

**BACKGROUND**: Want to minimize the run-time dependencies for a shared library to reduce risk of compatibility problems.

## Documentation
* GCC [Options for Code Generation Conventions](https://gcc.gnu.org/onlinedocs/gcc/Code-Gen-Options.html) - covers `-fvisibility=hidden`
* GCC [Options Controlling C++ Dialect](https://gcc.gnu.org/onlinedocs/gcc/C_002b_002b-Dialect-Options.html) - covers `-fvisibility-inlines-hidden`
* GCC [Symbol Visibility](https://gcc.gnu.org/wiki/Visibility) wiki
* GCC [Options for Linking](https://gcc.gnu.org/onlinedocs/gcc/Link-Options.html) - covers `-static-libgcc` and `-static-libstdc++`
* GNU binutils [LD](https://sourceware.org/binutils/docs/ld.html) - covers `--exclude-libs`
