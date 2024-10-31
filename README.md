# Corvus 3D - Vulkan Game Engine  
  

![Preview from 08-10-2022](https://github.com/sltn011/Corvus/blob/main/PreviewImages/31-12-2022.png)  
  
Corvus is a 3D Game Engine currenlty being developed for x64 Windows, designed to be a lightweight solution for making high performance games with high quality graphics.  

Features:
- Vulkan Renderer
- Layers based architecture
- Custom high perfomance system of memory pools for game objects creation
- Single and Multicast delegates  
- Loading models and their materials using gltf 2.0 files
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
  
# Getting Started  
## 0. Download and install Vulkan SDK  
Download Vulkan SDK 1.3 from https://vulkan.lunarg.com/sdk/home and install it on your PC.  
**Make sure that path to SDK was added to your PATH system variable!**  

## 1. Download repository  
Clone repository using `git clone --recursive https://github.com/sltn011/Corvus.git` to download it with submodules  
If repository was cloned non-recursive, open project folder and run ```git submodule update --init``` to download submodules  
  
## 2. Generate project files  
Go to Scripts folder and run Setup.bat for your IDE  
Generated Corvus.sln solution file can be found in project root folder.  
