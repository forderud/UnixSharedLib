## Automatic loading of static and shared libraries

#### Demonstrates the folllowing
* How to link to a dylib/shared library so that it`s automatically loaded at startup without first being called.
* How to link to an entire static library, including symbols that are not accessed.
* How to automatically run a function at startup when a shared library is loaded.

## Relevant C/C++ attributes
* `__attribute__((constructor))`: Run function at startup _before_ calling `main()`, or when the shared-lib is loaded.
* `__attribute__((used))`: Prevent linker from removing unreferenced function or global variable, despite it being unreferenced.
* `__attribute__((visibility("default")))`: Export function or global variable to make it accessible from outside the library.

## Relevant linker flags

#### For static libs:
* Linux: `-Wl,--whole-archive -lStaticLibName -Wl,--no-whole-archive` ([ld linker doc](https://man7.org/linux/man-pages/man1/ld.1.html)
* Mac: `-Wl,-force_load,libStaticLibName.a`

Since CMake 3.24, `$<LINK_LIBRARY:WHOLE_ARCHIVE,...>` provides a portable way to do the same.

#### For shared libs:
* Linux: `-Wl,--no-as-needed SharedLibName -Wl,--as-needed` ([ld linker doc](https://man7.org/linux/man-pages/man1/ld.1.html)
* Mac: `-Wl,-needed_library SharedLibName`

## Useful CMake features
* [`WHOLE_ARCHIVE`](https://cmake.org/cmake/help/latest/variable/CMAKE_LANG_LINK_LIBRARY_USING_FEATURE.html) available in CMake >=3.24
* [`NEEDED_LIBRARY`](https://cmake.org/cmake/help/latest/variable/CMAKE_LANG_LINK_LIBRARY_USING_FEATURE.html) available in CMake >=3.24
