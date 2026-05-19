echo Cleaning up...
rm -f *.o mainApp


echo Building mainApp...
g++ main.cpp -o mainApp

echo ""
echo Running mainApp:
./mainApp
