echo Cleaning up...
rm MyExecutable


echo Building MyExecutable independently...
# add -v to debug include problems
FRAMEWORK_DIR=`pwd`/../build/MySharedLib
gcc -F$FRAMEWORK_DIR -rpath $FRAMEWORK_DIR -framework MySharedLib -o MyExecutable Main.cpp

echo Running MyExecutable...
./MyExecutable
