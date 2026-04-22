echo Cleaning up...
rm -f *.o *.so mainApp


if [ "$(uname)" = "Darwin" ]; then
    echo "Running on macOS (Darwin)."
    REMOVE_UNREF=""
    OPTIMIZE="-O2 -flto"
    RPATH=""
else
    echo "Running on Linux."
    REMOVE_UNREF="-ffunction-sections -fdata-sections -Wl,--gc-sections"
    OPTIMIZE="-O2 -flto -fwhole-program"
    RPATH="-Wl,-rpath=."
fi

echo ""
echo Building libmylib.so...
g++ -fPIC -fvisibility=hidden $REMOVE_UNREF -c mylib.cpp -o mylib.o
g++ -shared -o libmylib.so mylib.o

echo Building mainApp...
# Might also need to use the following parameters:
# * Linux: -Wl,--no-as-needed
# * Mac  : -Wl,-needed_library,/path/to/libmylib.dylib
g++ main.cpp $OPTIMIZE $REMOVE_UNREF -L. -lmylib $RPATH -o mainApp

echo ""
echo Running mainApp:
./mainApp
