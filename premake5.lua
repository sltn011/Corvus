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
	
include "Corvus"
include "CorvusEditor"
include "CorvusEditorApp"
include "Playground"


group "Dependencies"
	include "Corvus/Dependencies/GLFW"
	include "Corvus/Dependencies/GLAD"
	include "Corvus/Dependencies/imgui"
	include "Corvus/Dependencies/ImGuizmo"
group ""
	