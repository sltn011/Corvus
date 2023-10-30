@echo off

set VulkanPath=%VULKAN_SDK%

"%VulkanPath%\Bin\glslc.exe" .\Deferred.vert -o Deferred.vert.spv
"%VulkanPath%\Bin\glslc.exe" .\Deferred.frag -o Deferred.frag.spv

"%VulkanPath%\Bin\glslc.exe" .\Combine.vert -o Combine.vert.spv
"%VulkanPath%\Bin\glslc.exe" .\Combine.frag -o Combine.frag.spv

pause
