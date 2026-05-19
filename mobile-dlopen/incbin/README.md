Test of using https://github.com/graphitemaster/incbin for binary file embeding. This can be a nice short-term approach until [`#embed`](https://en.cppreference.com/w/cpp/preprocessor/embed.html) becomes available in C++26.

## Linux & Android
Everything seem to work fine, with no observed problems.

Not yet tested on Android.

## MacOS & iOS
Observed build warning:
```
In file included from main.cpp:2:
./incbin.h:166:6: warning: "incbin is incompatible with bitcode. Using the library will break upload to App Store if you have bitcode enabled. Add `#define INCBIN_SILENCE_BITCODE_WARNING` before including this header to silence this warning." [-W#warnings]
  166 | #    warning "incbin is incompatible with bitcode. Using the library will break upload to App Store if you have bitcode enabled. Add `#define INCBIN_SILENCE_BITCODE_WARNING` before including this header to silence this warning."
      |      ^
1 warning generated.
```

Have submitted [incbin PR #72](https://github.com/graphitemaster/incbin/pull/72) to fix this annoyance. May need to use `ld -r -b binary` instead if still relevant.

## Windows
Not attractive, since adoption will require adoption of an ad-hoc `incbin.exe` preprocessor tool. It's then better to instead rely on `RCDATA` embeding for the time being.

## WASM
Will be supported with [incbin PR #70](https://github.com/graphitemaster/incbin/pull/70). Can use `--embed-file` meanwhile.
