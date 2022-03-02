--premake5.lua

project "Corvus"
	language "C++"
	cppdialect "C++17"
	kind "StaticLib"
	staticruntime "Off"
	
	targetdir ("%{wks.location}/Binary/" .. outputpath .. "/%{prj.name}")
	objdir ("%{wks.location}/Binary-Intermediate/" .. outputpath .. "/%{prj.name}")
	
	pchheader "CorvusPCH.h"
	pchsource "Source/CorvusPCH.cpp"
	
	files
	{
		"Source/**.cpp",
		"Source/**.h"
	}
	vpaths 
	{ 
		["../Corvus/Source/*"] = 
		{ 
			"Source/**.cpp",
			"Source/**.h" 
		} 
	}
	
	includedirs
	{
		"Source",
		"Dependencies/spdlog/include"
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
	