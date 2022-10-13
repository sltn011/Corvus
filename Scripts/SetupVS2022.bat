@echo off
cd ..\
echo Setting up submodules...
git submodule update --init
cd .\Scripts\Premake
call RunPremakeVS2022.bat
PAUSE
