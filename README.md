
Sample code project listing:

| Module             | Description                                             |
|--------------------|---------------------------------------------------------|
| **embed-file**     | Embedding arbitrary files into shared libraries and executables |
| **force-load**     | Forced loading of static and shared libraries           |
|**static-lib-embed**| Merge multiple static libraries into one static library |

## Linux resources

### C/C++ libraries bundled with Linux distros

| Distro                        | Extended support| Linux kernel | Glibc | GCC/libstdc++ |
|-------------------------------|-----------------|--------------|-------|---------------|
| RedHat 8.8 - 8.10             |    '19 -     '29| 4.18         | 2.28  | 8             |
| [Ubuntu 18.04 (Bionic Beaver)](https://releases.ubuntu.com/18.04/)   |Apr '18 - Apr '28| 4.15         | 2.27  | 7             |
| [Ubuntu 22.04 (Jammy Jellyfish)](https://releases.ubuntu.com/22.04/) |Apr '22 - Apr '32| 5.15 or 5.17 | 2.35  | 11            |
| [Ubuntu 24.04 (Noble Numbat)](https://releases.ubuntu.com/24.04/)    |Apr '24 - Apr '34| 6.8          | 2.39  | 13            |
| [Ubuntu 26.04 (Resolute Raccoon)](https://releases.ubuntu.com/26.04/)|Apr '26 - TBD   |              |       |               |
| Fedora 43                     |Oct '25 - Dec '26| 6.17         | 2.42  | 15            |

Please note that the Linux kernel version is determined by the host when running Docker containers, since containers doesn't contain their own kernel.

Kernel versions taken from Wikipedia [Ubuntu version history](https://en.wikipedia.org/wiki/Ubuntu_version_history#Table_of_versions) and [Fedora Linux release history](https://en.wikipedia.org/wiki/Fedora_Linux_release_history).

### Glibc/GCC links
* Glibc [Release Timeline](https://sourceware.org/glibc/wiki/Glibc%20Timeline)
* Glibc [README](https://github.com/bminor/glibc/blob/master/README) (glibc 2.24-2.42 require Linux 3.2 or newer that was released back in 2012)
* [GCC Releases](https://gcc.gnu.org/releases.html) with dates
* GNU C++ Library Manual: [Chapter 2. Setup](https://github.com/gcc-mirror/gcc/blob/master/libstdc%2B%2B-v3/doc/html/manual/setup.html) (glibc 2.3 or later is required for GCC >=4.6)
* [C++ Standards Support in GCC](https://gcc.gnu.org/projects/cxx-status.html)

### Distro packages
* Ubuntu [`glibc` package](https://launchpad.net/ubuntu/+source/glibc), [`g++` package](https://packages.ubuntu.com/noble/g++)
* Fedora [`glibc` package](https://packages.fedoraproject.org/pkgs/glibc/glibc/), [`libstdc++` package](https://packages.fedoraproject.org/pkgs/gcc/libstdc++/)
* DistroWatch: https://distrowatch.com/table.php?distribution=redhat (glibc 2.28 since RHEL 8.10 and glibc 2.17 since RHEL 7.9)
* [Red Hat Enterprise Linux Life Cycle](https://access.redhat.com/support/policy/updates/errata) (RHEL 8 is supported from 2019-2029)

## Other links
* CMake [cmake-toolchains](https://cmake.org/cmake/help/latest/manual/cmake-toolchains.7.html) with cross-compile documentation.
* GCC demangler: https://www.demangler.com/
