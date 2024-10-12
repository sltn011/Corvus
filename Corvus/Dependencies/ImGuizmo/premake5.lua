--premake5.lua

project "ImGuizmo"
    language "C++"
    cppdialect "C++20"
    kind "StaticLib"
    staticruntime "Off"
    
    targetdir ("%{wks.location}/Binary/" .. outputpath .. "/%{prj.name}")
    objdir ("%{wks.location}/Binary-Intermediate/" .. outputpath .. "/%{prj.name}")
    
    includedirs
    {
        "Source",
        "%{wks.location}/Corvus/Dependencies/imgui/Source"
    }
    
    files
    {
        "Source/ImGuizmo.h",
        "Source/ImGuizmo.cpp"
    }
    
    links
    {
        "imgui"
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
    