echo Cleaning up...
rm MyExecutable


echo Building MyExecutable independently...
# add -v to debug include problems
gcc -F/Users/forderud/VBoxShare/UnixSharedLib/embed-file/mac-framework/build/MyDynFramework -framework MyDynFramework -o MyExecutable Main.cpp

echo Running MyExecutable...
./MyExecutable
