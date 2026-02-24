### C/C++ libraries bundled with Linux distros

| Distro                        | Support life    | Linux kernel | Glibc | GCC/libstdc++ |
|-------------------------------|-----------------|--------------|-------|---------------|
| Ubuntu 18.04 (Bionic Beaver)  |Apr '18 - Apr '28| 4.15         | 2.27  | 7             |
| Ubuntu 22.04 (Jammy Jellyfish)|Apr '22 - Apr '32| 5.15 or 5.17 |       |               |
| Ubuntu 24.04 (Noble Numbat)   |Apr '24 - Apr '34| 6.8          | 2.39  | 13            |
| Fedora 43                     |Oct '25 - Dec '26| 6.17         | 2.42  | 15            |

Kernel versions taken from Wikipedia [Ubuntu version history](https://en.wikipedia.org/wiki/Ubuntu_version_history#Table_of_versions) and [Fedora Linux release history](https://en.wikipedia.org/wiki/Fedora_Linux_release_history).

### Glibc/GCC links
* Glibc [Release Timeline](https://sourceware.org/glibc/wiki/Glibc%20Timeline)
* Glibc [README](https://github.com/bminor/glibc/blob/master/README) (glibc 2.24-2.42 require Linux 3.2 or newer)
* [GCC Releases](https://gcc.gnu.org/releases.html) with dates
* GNU C++ Library Manual: [Chapter 2. Setup](https://github.com/gcc-mirror/gcc/blob/master/libstdc%2B%2B-v3/doc/html/manual/setup.html) (glibc 2.3 or later is required for GCC >=4.6)
* [C++ Standards Support in GCC](https://gcc.gnu.org/projects/cxx-status.html)

### Distro packages
* Ubuntu [`glibc` package](https://launchpad.net/ubuntu/+source/glibc), [`g++` package (24.04)](https://launchpad.net/ubuntu/noble/+package/g++)
* Fedora [`glibc` package](https://packages.fedoraproject.org/pkgs/glibc/glibc/), [`libstdc++` package](https://packages.fedoraproject.org/pkgs/gcc/libstdc++/)
* DistroWatch: https://distrowatch.com/table.php?distribution=redhat (glibc 2.28 since RHEL 8.10 and glibc 2.17 since RHEL 7.9)
* [Red Hat Enterprise Linux Life Cycle](https://access.redhat.com/support/policy/updates/errata) (RHEL 8 is supported from 2019-2029)
