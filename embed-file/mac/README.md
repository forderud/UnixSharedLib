## Mach-O embed and parsing samples

#### Demonstrates the folllowing
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
