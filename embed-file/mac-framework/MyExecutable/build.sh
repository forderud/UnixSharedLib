echo Cleaning up...
rm MyExecutable


echo Building MyExecutable independently...
# add -v to debug include problems
FRAMEWORK_DIR=/Users/forderud/VBoxShare/UnixSharedLib/embed-file/mac-framework/build/MyDynFramework
gcc -F$FRAMEWORK_DIR -rpath $FRAMEWORK_DIR -framework MyDynFramework -o MyExecutable Main.cpp

echo Running MyExecutable...
./MyExecutable
