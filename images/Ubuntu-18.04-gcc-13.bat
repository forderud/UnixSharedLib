@echo off

echo Building Ubuntu 18.04 image with GCC 13...
::docker build -f Ubuntu-18.04-gcc-13.Dockerfile --tag=unixsharedlibs:18.04-gcc-13 .

echo Building sample project...
docker run --rm -v %cd%:/host unixsharedlibs:18.04-gcc-13 /host/bundle-libstdc++/build_lib.sh || exit /b 1

:: Open an interactive bash shell inside the container
::docker run --rm -it unixsharedlibs:18.04-gcc-13 sh
