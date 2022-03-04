--premake5.lua

project "GLAD"
	language "C"
	kind "StaticLib"
	staticruntime "Off"
	
	targetdir ("%{wks.location}/Binary/" .. outputpath .. "/%{prj.name}")
	objdir ("%{wks.location}/Binary-Intermediate/" .. outputpath .. "/%{prj.name}")
	
	includedirs
	{
		"Source/include"
	}
	
	files
	{
		"Source/include/glad/glad.h",
		"Source/include/KHR/khrplatform.h",
		"Source/src/glad.c"
	}
	
	filter { "configurations:Debug" }
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
	