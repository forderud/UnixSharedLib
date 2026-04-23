## Automatic loading of shared libraries

#### Demonstrates the folllowing
* How to link to a dylib/shared library so that it`s automatically loaded at startup without having to first call an exported function.
* How to automatically run a function when a shared library is loaded.

## Useful CMake features
* [`WHOLE_ARCHIVE`](https://cmake.org/cmake/help/latest/variable/CMAKE_LANG_LINK_LIBRARY_USING_FEATURE.html) available in CMake >=3.24
* [`NEEDED_LIBRARY`](https://cmake.org/cmake/help/latest/variable/CMAKE_LANG_LINK_LIBRARY_USING_FEATURE.html) available in CMake >=3.24
