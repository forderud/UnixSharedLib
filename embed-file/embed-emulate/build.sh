echo Cleaning up...
rm -f *.o mainApp


echo Building mainApp...
g++ -std=c++23 main.cpp -o mainApp

echo ""
echo Running mainApp:
./mainApp
