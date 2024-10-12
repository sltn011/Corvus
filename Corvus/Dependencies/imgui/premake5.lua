--premake5.lua

project "imgui"
	language "C++"
	cppdialect "C++20"
	kind "StaticLib"
	staticruntime "Off"
	
	targetdir ("%{wks.location}/Binary/" .. outputpath .. "/%{prj.name}")
	objdir ("%{wks.location}/Binary-Intermediate/" .. outputpath .. "/%{prj.name}")
	
	includedirs
	{
		"Source"
	}
	
	files
	{
		"Source/imgui.h",
		"Source/imgui.cpp",
		
		"Source/imconfig.h",
		"Source/imgui_demo.cpp",
		"Source/imgui_draw.cpp",
		"Source/imgui_internal.h",
		"Source/imgui_tables.cpp",
		"Source/imgui_widgets.cpp",
		"Source/imstb_rectpack.h",
		"Source/imstb_textedit.h",
		"Source/imstb_truetype.h"
	}	
	
	filter { "system:windows" }
		defines
		{
			"_CRT_SECURE_NO_WARNINGS"
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
	