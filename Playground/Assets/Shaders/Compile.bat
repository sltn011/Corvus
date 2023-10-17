@echo off

set VulkanPath=%VULKAN_SDK%

"%VulkanPath%\Bin\glslc.exe" .\Shader.vert -o vert.spv
"%VulkanPath%\Bin\glslc.exe" .\Shader.frag -o frag.spv

pause
