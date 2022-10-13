@echo off
cd ..\
echo Setting up submodules...
git submodule update --init
cd .\Scripts\Premake
call RunPremakeVS2019.bat
PAUSE
