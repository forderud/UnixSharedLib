@echo off

echo Building Fedora 43 image...
::docker build -f Fedora-43.Dockerfile --tag=unixsharedlibs:fedora .

echo Building sample project...
docker run --rm -v %cd%:/host unixsharedlibs:fedora /host/bundle-libstdc++/build_app.sh || exit /b 1

:: Open an interactive bash shell inside the container
::docker run --rm -it unixsharedlibs:fedora sh
