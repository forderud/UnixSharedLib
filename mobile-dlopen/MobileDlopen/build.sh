echo Cleaning up...
rm MobileDlopen


echo Building MobileDlopen independently...
# add -v to debug include problems
FRAMEWORK_DIR=`pwd`/../build/MySharedLib
gcc -F$FRAMEWORK_DIR -rpath $FRAMEWORK_DIR -framework MySharedLib -o MobileDlopen Main.cpp

echo Running MobileDlopen...
./MobileDlopen
