Test of using https://github.com/graphitemaster/incbin for binary file embeding

## Linux & Android
Everything seem to work fine, with no observed problems.

Not yet tested on Andriod.

## MacOS & iOS
Observed build warning:
```
In file included from main.cpp:2:
./incbin.h:166:6: warning: "incbin is incompatible with bitcode. Using the library will break upload to App Store if you have bitcode enabled. Add `#define INCBIN_SILENCE_BITCODE_WARNING` before including this header to silence this warning." [-W#warnings]
  166 | #    warning "incbin is incompatible with bitcode. Using the library will break upload to App Store if you have bitcode enabled. Add `#define INCBIN_SILENCE_BITCODE_WARNING` before including this header to silence this warning."
      |      ^
1 warning generated.
```

This is a blocker if still relevant, since it will prevent usage in Apple App store apps. Have submitted [incbin PR #72](https://github.com/graphitemaster/incbin/pull/72) to clarify the current state. May need to use `ld -r -b binary` instead if still relevant.

## Windows
Not attractive, since adoption will require adoption of an ad-hoc `incbin.exe` preprocessor tool. It's then better to instead rely on `RCDATA` embeding.

## WASM
Unsupported platform ([issue #37](https://github.com/graphitemaster/incbin/issues/37)). Will probably need to use `--embed-file` instead.
