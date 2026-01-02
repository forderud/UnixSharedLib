echo Cleaning up...
rm -f *.o mainApp


echo Building mainApp...
g++ -std=c++2c -Wpre-c++23-compat -Wc23-extensions main.cpp -o mainApp

echo ""
echo Running mainApp:
./mainApp
