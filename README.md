# Corvus
Corvus 3D Game Engine for Windows
  
# Getting Started  
You can just download zip and run Setup.bat file.   
  
OR    
## 1. Download repository  
Clone repository using `git clone --recursive https://github.com/sltn011/Corvus.git` to download it with submodules  
If repository was cloned non-recursive, open project folder and run ```git submodule update --init``` to download submodules  
  
## 2. Generate project files  
Run these commands inside project using command prompt:
```bat
"./Dependencies/Premake/Binary/premake5.exe" vs2019
```  
Generated solution file can be found in project root folder.  
