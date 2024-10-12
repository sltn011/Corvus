--premake5.lua

project "Corvus"
    language "C++"
    cppdialect "C++20"
    kind "StaticLib"
    staticruntime "Off"
    
    targetdir ("%{wks.location}/Binary/" .. outputpath .. "/%{prj.name}")
    objdir ("%{wks.location}/Binary-Intermediate/" .. outputpath .. "/%{prj.name}")
    
    pchheader "CorvusPCH.h"
    pchsource "Source/CorvusPCH.cpp"
    
    files
    {
        "Source/**.cpp",
        "Source/**.h",
        
        "Dependencies/glm/glm/**.hpp",
        "Dependencies/glm/glm/**.inl"
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
        "Dependencies/spdlog/include",
        "Dependencies/GLFW/Source/include",
        "Dependencies/GLAD/Source/include",
        "Dependencies/imgui/Source",
        "Dependencies/ImGuizmo/Source",
        "Dependencies/glm",
        "Dependencies/stb",
        "Dependencies/json",
        "Dependencies/tiny"
    }
    
    links
    {
        "GLFW",
        "GLAD",
        "imgui",
        "ImGuizmo"
    }
    
    defines
    {
        "GLFW_INCLUDE_NONE",
        "_CRT_SECURE_NO_WARNINGS"
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
        defines
        {
            "CORVUS_RELEASE"
        }
        
        flags
        {
            "MultiProcessorCompile"
        }
        
        optimize "On"
        symbols "Off"
        
    filter{}
    