@echo off

echo Building RedHat 8 image...
::docker build -f RedHat-8.Dockerfile --tag=unixsharedlibs:redhat8 .

::echo Building sample project...
::docker run --rm -v %cd%:/host unixsharedlibs:redhat8 /host/bundle-libstdc++/build_app.sh || exit /b 1

:: Open an interactive bash shell inside the container
docker run --rm -it unixsharedlibs:redhat8 sh
