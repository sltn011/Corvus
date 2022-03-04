--premake5.lua

project "GLFW"
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
		"Source/include/GLFW/glfw3.h",
		"Source/include/GLFW/glfw3native.h",
		
		"Source/src/internal.h",
		"Source/src/platform.h",
		"Source/src/mappings.h",
		"Source/src/context.c",
		"Source/src/init.c", 
		"Source/src/input.c", 
		"Source/src/monitor.c", 
		"Source/src/platform.c", 
		"Source/src/vulkan.c", 
		"Source/src/window.c",
		"Source/src/egl_context.c", 
		"Source/src/osmesa_context.c", 
		"Source/src/null_platform.h", 
		"Source/src/null_joystick.h",
		"Source/src/null_init.c", 
		"Source/src/null_monitor.c", 
		"Source/src/null_window.c", 
		"Source/src/null_joystick.c"
	}
	
	filter { "system:windows" }
		defines
		{
			"_GLFW_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
		}
		
		files
		{
			"Source/src/win32_platform.h",
			"Source/src/win32_joystick.h", 
			"Source/src/win32_init.c",
			"Source/src/win32_joystick.c",
			"Source/src/win32_monitor.c", 
			"Source/src/win32_window.c",
			"Source/src/wgl_context.c",
			"Source/src/win32_time.h", 
			"Source/src/win32_thread.h", 
			"Source/src/win32_module.c",
			"Source/src/win32_time.c", 
			"Source/src/win32_thread.c",
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
	