Example of bundling a new `libstdc++` version with a shared library to allow it to run on older Linux distros.

### g++ libstdc++ linking problem (solved)
It seems like `g++` will be default link to the libstdc++ version distributed with the compiler. This causes problem when one instead want to link to a newer libstdc++ version.

Build error g++ implicitly links to a too old libstdc++ version:
```
./libModernStuff.so: undefined reference to `std::__exception_ptr::exception_ptr::_M_release()@CXXABI_1.3.13'
./libModernStuff.so: undefined reference to `std::__exception_ptr::exception_ptr::_M_addref()@CXXABI_1.3.13'
./libModernStuff.so: undefined reference to `std::__throw_bad_array_new_length()@GLIBCXX_3.4.29'
collect2: error: ld returned 1 exit status
```

| Solution | Description |
|-------------|-------------|
| GCC ≥ 13   | Use [`-nostdlib++`](https://gcc.gnu.org/onlinedocs/gcc/Link-Options.html#index-nostdlib_002b_002b). |
| GCC < 13    | Pass [`-nodefaultlibs`](https://gcc.gnu.org/onlinedocs/gcc/Link-Options.html#index-nodefaultlibs) and link to required C standard libraries. Example: ``-nodefaultlibs -lgcc_eh -lc `pwd`/libstdc++.so.6`` to suppress automatic libstdc++ linking and instead link to the user-provided `libstdc++.so.6`. |

Response by RedHat employee and libstdc++ maintainer Jonathan Wakely to: "*g++: How to link against custom libstdc++ version?*" [response](https://gcc.gnu.org/pipermail/libstdc++/2025-November/064446.html): "*It should work to use -nostdlib++ and add the full path to the newer libstdc++.so.6*."

## Links
* [Multiple glibc libraries on a single host](https://stackoverflow.com/questions/847179/multiple-glibc-libraries-on-a-single-host) - mentions `-Wl,--rpath`/`-Wl,--dynamic-linker` and `LD_LIBRARY_PATH`/`LD_PRELOAD`
* GNU binutils [LD](https://sourceware.org/binutils/docs/ld.html) doc - covers `-rpath`/`LD_RUN_PATH`
* GCC [3.16 Options for Linking](https://gcc.gnu.org/onlinedocs/gcc/Link-Options.html) - covers `-l`
* GCC [3.17 Options for Directory Search](https://gcc.gnu.org/onlinedocs/gcc/Directory-Options.html) - covers `-L`
* [GCC 13](https://gcc.gnu.org/gcc-13/changes.html) introduced a new `-nostdlib++` option
* GCC C++ [ABI Policy and Guidelines](https://gcc.gnu.org/onlinedocs/libstdc++/manual/abi.html)
* GCC [3.4 Options Controlling C Dialect](https://gcc.gnu.org/onlinedocs/gcc/C-Dialect-Options.html) - covers `-std=`
* GCC [3.5 Options Controlling C++ Dialect](https://gcc.gnu.org/onlinedocs/gcc/C_002b_002b-Dialect-Options.html) - covers `-fabi-version=n` and `-fabi-compat-version=n`
