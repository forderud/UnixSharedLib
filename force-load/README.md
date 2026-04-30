## Forced loading of static and shared libraries

#### Demonstrates the folllowing
* How to link to a dylib/shared library so that it`s automatically loaded at startup _without_ first being called.
  - [x] For Linux
  - [x] For Mac
  - [x] For iOS
* How to link to an entire static library, including symbols that are _not_ accessed.
  - [x] For Linux
  - [x] For Mac
  - [x] For iOS
  - [x] For Emscripten (WebAssembly)
* [x] How to automatically run a function at startup _before_ `main()` is called, or when the shared-lib is loaded.

## Related compiler attributes
* [`__attribute__((visibility("default")))`](https://gcc.gnu.org/onlinedocs/gcc/Common-Attributes.html#index-visibility): Export function or global variable to make it accessible from outside the library.
* [`__attribute__((constructor))`](https://gcc.gnu.org/onlinedocs/gcc/Common-Attributes.html#index-constructor): Run function at startup _before_ `main()` is called, or when the shared-lib is loaded.
* [`__attribute__((used))`](https://gcc.gnu.org/onlinedocs/gcc/Common-Attributes.html#index-used): Prevent linker from removing unreferenced function or global variable, despite it being unreferenced.

## Related linker options

#### For static libs:
* **Linux**: `-Wl,--whole-archive -lStaticLibName -Wl,--no-whole-archive`. Alternatively, `-Wl,--require-defined=<symbol>` to force-include a selected symbol ([ld linker doc](https://man7.org/linux/man-pages/man1/ld.1.html))
* **Mac/iOS**: `-Wl,-force_load libStaticLibName.a`. Alternatively, `-Wl,-u,_<symbol>` to force-include a selected symbol.
* **Emscripten**: `-Wl,--undefined=<symbol>` to force-include a selected symbol

Since CMake 3.24, [`$<LINK_LIBRARY:WHOLE_ARCHIVE,StaticLibName>`](https://cmake.org/cmake/help/latest/manual/cmake-generator-expressions.7.html) provides a portable way to do the same.

#### For shared libs:
* **Linux**: `-Wl,--no-as-needed -lSharedLibName -Wl,--as-needed` ([ld linker doc](https://man7.org/linux/man-pages/man1/ld.1.html))
* **Mac/iOS**: `-Wl,-needed_library libSharedLibName.dylib`

Since CMake 3.24, [`$<LINK_LIBRARY:NEEDED_LIBRARY,SharedLibName>`](https://cmake.org/cmake/help/latest/manual/cmake-generator-expressions.7.html) provides a convenient way to do the same for Apple platforms.

## Dependency checking

Command for listing shared-lib dependencies:
* **Linux**: `ldd <binary-file>`
* **Mac**: `otool -L <binary-file>`
