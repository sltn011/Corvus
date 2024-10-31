@echo off

set VulkanPath=%VULKAN_SDK%

"%VulkanPath%\Bin\glslc.exe" -I .\include .\Deferred.vert -o Deferred.vert.spv
"%VulkanPath%\Bin\glslc.exe" -I .\include .\Deferred.frag -o Deferred.frag.spv

"%VulkanPath%\Bin\glslc.exe" -I .\include .\Combine.vert -o Combine.vert.spv
"%VulkanPath%\Bin\glslc.exe" -I .\include .\Combine.frag -o Combine.frag.spv

"%VulkanPath%\Bin\glslc.exe" -I .\include .\Postprocess.vert -o Postprocess.vert.spv
"%VulkanPath%\Bin\glslc.exe" -I .\include .\Postprocess.frag -o Postprocess.frag.spv

pause
