## ELF & Mach-O file embed and parsing samples

Sample code for embedding arbitrary binary or text files into binaries on various platforms.

### Platform support
| Platform | Approach |
|----------|----------------|
| Windows  | Use [`RCDATA`](https://learn.microsoft.com/en-us/windows/win32/menurc/rcdata-resource) resources to embed arbitrary files into a `.dll` or `.exe`. |
| Linux/Android | Add a named `.symtab` symbol to the `.so` binary. |
| macOS/iOS     | Add a named `__TEXT` symbol to the `.dylib` binary.|

### Cross-platform alternative
[`#embed`](https://en.cppreference.com/cpp/preprocessor/embed) that's introduced in C23/C++26 can be used as portable solution for arbitrary file embedding. There  **incbin** sample furthermore demonstrates how achieve the same with pre-C23/C++26 compilers.

#### Demonstrates the following
* Embed arbitrary binary files in shared libraries
* Access the embedded files without running the binary

## Documentation
* [OS X ABI Mach-O File Format Reference](https://github.com/aidansteele/osx-abi-macho-file-format-reference)
* https://lowlevelbits.org/parsing-mach-o-files/
* [Mach-O Viewer](https://github.com/terrakok/Mach-O-viewer)
* CMake [FRAMEWORK](https://cmake.org/cmake/help/latest/prop_tgt/FRAMEWORK.html) support
* [`dlopen` limitations on iOS](https://stackoverflow.com/questions/17829201/is-dlopen-use-inside-a-static-library-in-ios-allowed) - Using dlopen with literal parameters is ok
* Mach-O file format [4GB Limit](https://jonasdevlieghere.com/post/macho-4gb-limit/) also in 64bit

### Samples
* GitHubb [sheldonth/ios-cmake](https://github.com/sheldonth/ios-cmake) sample with dynamic framwork sample
