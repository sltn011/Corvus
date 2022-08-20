# Corvus 3D Game Engine  
  

![Preview from 30-07-2022](https://github.com/sltn011/Corvus/blob/main/PreviewImages/30-07-2022.png)  
  
Corvus is a 3D Game Engine currenlty being developed for x64 Windows, designed to be a lightweight solution for making high performance games with high quality graphics.  
Engine uses OpenGL as it's Rendering API for now, but is built to allow easy integration of other APIs and Platforms support in future.  

Features:
- Layers based architecture
- Custom high perfomance system of memory pools for game objects creation
- Single and Multicast delegates
- Log and assert debug tools
  
Planned features:
- ECS based on custom pools system
- PBR, Bloom, Dynamic Shadows and Reflections and other high quality graphics related effects and systems
- Custom Editor
- Blueprints based materials system
- Physics
- Skeletal animations
- Scripting
- Networking
- Support for other Rendering APIs and Platforms
  
# Getting Started     
## 1. Download repository  
Clone repository using `git clone --recursive https://github.com/sltn011/Corvus.git` to download it with submodules  
If repository was cloned non-recursive, open project folder and run ```git submodule update --init``` to download submodules  
  
## 2. Generate project files  
Go to Scripts folder and run Setup.bat or  
Run these commands inside project root directory using command prompt:
```bat
"./Dependencies/Premake/Binary/premake5.exe" vs2022
```  
Generated solution file can be found in project root folder.  
