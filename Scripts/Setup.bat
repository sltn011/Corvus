@echo off
cd ..\
echo Setting up submodules...
git submodule update --init
cd .\Scripts
call RunPremake.bat
PAUSE
