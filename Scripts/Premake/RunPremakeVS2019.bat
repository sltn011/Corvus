@echo off
cd ..\..\
@echo Creating solution files...
"./Dependencies/Premake/Binary/premake5.exe" vs2019
