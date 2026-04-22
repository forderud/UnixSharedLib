echo Cleaning up...
rm -f *.o *.so mainApp


if [ "$(uname)" = "Darwin" ]; then
    echo "Running on macOS (Darwin)."
    REMOVE_UNREF=""
    OPTIMIZE="-O2 -flto"
    FORCE_LINK="-Wl,-needed_library,libmylib.so"
    RPATH=""
else
    echo "Running on Linux."
    REMOVE_UNREF="-ffunction-sections -fdata-sections -Wl,--gc-sections"
    OPTIMIZE="-O2 -flto -fwhole-program"
    FORCE_LINK="-Wl,--no-as-needed"
    RPATH="-Wl,-rpath=."
fi

echo ""
echo Building libmylib.so...
g++ -fPIC -fvisibility=hidden $REMOVE_UNREF -c mylib.cpp -o mylib.o
g++ -shared -o libmylib.so mylib.o

echo Building mainApp...
g++ main.cpp $OPTIMIZE $REMOVE_UNREF $FORCE_LINK -L. -lmylib $RPATH -o mainApp

echo ""
echo Running mainApp:
./mainApp
