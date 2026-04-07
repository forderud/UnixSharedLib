Sample code for embedding arbitrary binary or text files into binaries on various platforms.

### Platform support
| Platform | Support status |
|----------|----------------|
| Windows  | Use [`RCDATA`](https://learn.microsoft.com/en-us/windows/win32/menurc/rcdata-resource) resources to embed arbitrary files into a `.dll` or `.exe`. |
| Linux/Android | Add a named `.symtab` symbol to the `.so` binary (see sample in `linux` subdirectory). |
| macOS/iOS     | Add a named `__TEXT` symbol to the `.dylib` binary  (see sample in `mac` subdirectory).|
