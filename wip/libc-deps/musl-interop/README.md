Test for calling a shared lib (`.so`) using the "musl" C library from a executable using the "glibc" C library.

**WARNING**: Acording to [Safe to load musl shared lib into glibc executable?](https://www.openwall.com/lists/musl/2025/11/12/6) then it's _not_ supported to link statically to musl when buliding a shared lib. 

## Musl binaries on Ubuntu
```
# ls -al /usr/bin/*musl*
-rwxr-xr-x 1 root root 440 Nov  9  2023 ld-musl-config
lrwxrwxrwx 1 root root  21 Nov 10  2023 musl-gcc -> x86_64-linux-musl-gcc
lrwxrwxrwx 1 root root  30 Nov 10  2023 musl-ldd -> /lib/x86_64-linux-musl/libc.so
-rwxr-xr-x 1 root root 106 Nov 10  2023 x86_64-linux-musl-gcc
```

```
#ls -al /lib/x86_64-linux-musl
total 3344
drwxr-xr-x 2 root root    4096 Nov 11 17:39 .
drwxr-xr-x 1 root root    4096 Nov 11 17:39 ..
-rw-r--r-- 1 root root    5384 Nov 10  2023 Scrt1.o
-rw-r--r-- 1 root root    5376 Nov 10  2023 crt1.o
-rw-r--r-- 1 root root    2632 Nov 10  2023 crti.o
-rw-r--r-- 1 root root    2576 Nov 10  2023 crtn.o
-rw-r--r-- 1 root root 2604238 Nov 10  2023 libc.a
-rwxr-xr-x 1 root root  731616 Nov 10  2023 libc.so
-rw-r--r-- 1 root root       8 Nov 10  2023 libdl.a
-rw-r--r-- 1 root root       8 Nov 10  2023 libm.a
-rw-r--r-- 1 root root       8 Nov 10  2023 libpthread.a
-rw-r--r-- 1 root root       8 Nov 10  2023 libresolv.a
-rw-r--r-- 1 root root       8 Nov 10  2023 librt.a
-rw-r--r-- 1 root root       8 Nov 10  2023 libutil.a
-rw-r--r-- 1 root root       8 Nov 10  2023 libxnet.a
-rw-r--r-- 1 root root     757 Nov 10  2023 musl-gcc.specs
-rw-r--r-- 1 root root   12696 Nov 10  2023 rcrt1.o
```

### Observed warnings
Warnings when building a non-musl application using `gcc` with musl in `LD_LIBRARY_PATH`:
```
/usr/bin/ld: /lib/x86_64-linux-musl/libc.so: warning: the use of `tmpnam' is dangerous, better use `mkstemp'
/usr/bin/ld: /lib/x86_64-linux-musl/libc.so: warning: the use of `mktemp' is dangerous, better use `mkstemp' or `mkdtemp'
/usr/bin/ld: /lib/x86_64-linux-musl/libc.so: warning: the `gets' function is dangerous and should not be used.
/usr/bin/ld: /lib/x86_64-linux-musl/libc.so: warning: the use of `tempnam' is dangerous, better use `mkstemp'
```

### Error if musl is _not_ in `LD_LIBRARY_PATH`
The following error is observed if forgetting to add musl to `LD_LIBRARY_PATH`:
```
./mainApp: error while loading shared libraries: /lib/x86_64-linux-gnu/libc.so: invalid ELF header
```

## Links
* musl [Functional differences from glibc](https://wiki.musl-libc.org/functional-differences-from-glibc) - `dlclose` not implemented
* musl mailing list: [Safe to load musl shared lib into glibc executable?](https://www.openwall.com/lists/musl/2025/11/12/5)
* [Load both musl libc.so and gcc libc.so in the same process?](https://stackoverflow.com/questions/30151808/load-both-musl-libc-so-and-gcc-libc-so-in-the-same-process): One answer claims that it's possible and one that it's unsupported. Therefore, need to investigate further.
