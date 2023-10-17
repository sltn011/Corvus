--premake5.lua

workspace "Corvus"

	architecture "x86_64"
	system "Windows"
	
	startproject "Playground"

	configurations 
	{
		"Debug",
		"Release" 
	}
	
	outputpath = "%{cfg.buildcfg}-%{cfg.architecture}"
	
	vulkanSDKpath = os.getenv("VULKAN_SDK")
	if vulkanSDKpath == nil then
		error("No VULKAN_SDK found in PATH! You must have VulkanSDK installed for code to compile and run!")
	end
	
	
	include "Corvus"
	include "Playground"
	
	
	group "Dependencies"
		include "Corvus/Dependencies/GLFW"
		include "Corvus/Dependencies/imgui"
	include "Corvus/Dependencies/ImGuizmo"
	group ""
	