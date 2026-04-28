@echo off

:: clean up any previous builds
rmdir build /s /q

mkdir build

:: Input sources must be found in current folder
:: Output binaries will be written to "build" subfolder

:: Valid choices are "Debug", "RelWithDebInfo", "Release" (also default)
set BUILD_TYPE=%1
set DOCKER_IMAGE=forderud/qtwasm:latest
set MOUNT_SOURCE=-v %cd%:/project/source
set MOUNT_BUILD=-v %cd%\build:/project/build

:: Default build settings
docker run --rm %MOUNT_SOURCE% %MOUNT_BUILD% %DOCKER_IMAGE% || exit /b 1
:: Don't expose sources through webserver
set MOUNT_SOURCE=

echo Opening web browser and starting web server...
start http://localhost:8080/build/MyExecutable/MyExecutable.html

:: Serve the webpage from the same docker container,
:: so that file paths match build-time paths.
docker run --rm -p 8080:8080 ^
    -v %cd%\WebServer.py:/project/WebServer.py ^
    %MOUNT_SOURCE% %MOUNT_BUILD% ^
    -w /project %DOCKER_IMAGE% ^
    python3 WebServer.py 8080
