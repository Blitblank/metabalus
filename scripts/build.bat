
rm -r build
mkdir build
cd build

cmake .. -G "MinGW Makefiles"
cmake --build .

C:\Qt\6.10.1\mingw_64\bin\windeployqt.exe HelloQt.exe
