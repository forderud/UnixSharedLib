@echo off

:: clean up any previous builds
rmdir build /s /q

mkdir build

:: Input sources must be found in current folder
:: Output binaries will be written to "build" subfolder

:: Valid choices are "Debug", "RelWithDebInfo", "Release" (also default)
set BUILD_TYPE=%1
set DOCKER_IMAGE=emscripten/emsdk:4.0.7
set MOUNT_SOURCE=-v %cd%:/src
set MOUNT_BUILD=-v %cd%\build:/src/build

:: Default build settings
docker run --rm %MOUNT_SOURCE% %MOUNT_BUILD% %DOCKER_IMAGE% sh -c "emcmake cmake -S . -B build; cmake --build build" || exit /b 1
:: Don't expose sources through webserver
set MOUNT_SOURCE=

echo Opening web browser and starting web server...
start http://localhost:8080/build/MyExecutable/MyExecutable.html

:: Serve the webpage from the same docker container,
:: so that file paths match build-time paths.
docker run --rm -p 8080:8080 ^
    -v %cd%\WebServer.py:/src/WebServer.py ^
    %MOUNT_SOURCE% %MOUNT_BUILD% ^
    -w /src %DOCKER_IMAGE% ^
    python3 WebServer.py 8080
