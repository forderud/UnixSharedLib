@echo off

echo RedHat 8 image...

:: Open an interactive bash shell inside the container
docker run --rm -it redhat/ubi8:8.8 sh
