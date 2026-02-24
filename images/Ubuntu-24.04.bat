@echo off

echo Building Ubuntu 24.04 image...
::docker build -f Ubuntu-24.04.Dockerfile --tag=unixsharedlibs:24.04 .

echo Building sample project...
docker run --rm -v %cd%:/host unixsharedlibs:24.04 /host/bundle-libstdc++/build.sh || exit /b 1

:: Open an interactive bash shell inside the container
::docker run --rm -it unixsharedlibs:24.04 sh
