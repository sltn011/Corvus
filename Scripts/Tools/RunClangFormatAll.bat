@echo off
cd ..\..\
echo Formatting Engine files...
for /r .\Corvus\Source %%f in (*.cpp, *.h) do (
	echo %%f
	clang-format -i %%f
)
echo Formatting Playground files...
for /r .\Playground\Source %%f in (*.cpp, *.h) do (
	echo %%f
	clang-format -i %%f
)
PAUSE
