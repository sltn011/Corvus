--premake5.lua

workspace "Corvus"

	architecture "x86_64"
	system "Windows"
	
	startproject "Sandbox"

	configurations 
	{
		"Debug",
		"Release" 
	}
	

outputpath = "%{cfg.buildcfg}-%{cfg.architecture}"
	
include "Corvus"
include "Sandbox"
	
	
	