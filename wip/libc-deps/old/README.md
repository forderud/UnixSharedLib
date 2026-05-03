The `glibc-2.40` subfolder contains a precompiled version of "mylib" built with [Fedora 41](https://fedoraproject.org/) (Linux 6.11, glibc 2.40) to simplify testing.

## Experiment
Experiment conducted on [Ubuntu 14.4](https://releases.ubuntu.com/14.04/) (Linux 4.4, glibc 2.19):
* Copy `mylib.o` and `libmylib.so` from glibc-2.40 subfolder to this folder.
* Disable "mylib" building step in `build.sh` so that the precompiled version is used.
* Run `build.sh` to build and run the test application.
* Observed output:
```
$ ./build.sh
./libmylib.so: undefined reference to `__isoc23_strtoul@GLIBC_2.38'
./libmylib.so: undefined reference to `fstat@GLIBC_2.33'
./libmylib.so: undefined reference to `getentropy@GLIBC_2.25'
./libmylib.so: undefined reference to `arc4random@GLIBC_2.36'
./libmylib.so: undefined reference to `pthread_once@GLIBC_2.34'
./libmylib.so: undefined reference to `_dl_find_object@GLIBC_2.35'
./libmylib.so: undefined reference to `__libc_single_threaded@GLIBC_2.32'
```

~~This demonstrates that **a "new" shared library compiled against glibc 2.39 can run out of the box on an older system with glibc 2.19**. The test succeeds both with implicit (`-lmylib`) and explicit (`dlopen("libmylib.so")`) loading of `libmylib.so`, which was a positive surprise.~~

There might be limitations though that are not yet understood. Still, I would assume that any problems would be easily discoverable, since missing dependencies typically lead to run-time crashes at _startup_, and not problems or crashes later during execution. A simple startup test should therefore suffice to assess compatibility with a given Linux distro release.
