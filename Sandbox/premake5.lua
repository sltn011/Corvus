--premake5.lua

project "Sandbox"
	language "C++"
	cppdialect "C++17"
	kind "ConsoleApp"
	staticruntime "Off"
	
	targetdir ("%{wks.location}/Binary/" .. outputpath .. "/%{prj.name}")
	objdir ("%{wks.location}/Binary-Intermediate/" .. outputpath .. "/%{prj.name}")
	
	files
	{
		"Source/**.cpp",
		"Source/**.h",
	}
	vpaths 
	{ 
		["../Sandbox/Source/*"] = 
		{ 
			"Source/**.cpp",
			"Source/**.h" 
		} 
	}
	
	includedirs
	{
		"%{wks.location}/Corvus/Source",
		"%{wks.location}/Corvus/Dependencies/spdlog/include",
		"%{wks.location}/Corvus/Dependencies/GLFW/Source/include",
		"%{wks.location}/Corvus/Dependencies/glm"
	}
	
	links
	{
		"Corvus"
	}
	
	filter { "configurations:Debug" }
		defines
		{
			"CORVUS_DEBUG"
		}
		
		flags
		{
			"MultiProcessorCompile"
		}
		
		symbols "On"
		
	filter { "configurations:Release" }
		flags
		{
			"MultiProcessorCompile"
		}
		
		optimize "On"
		symbols "Off"
		
	filter{}
	