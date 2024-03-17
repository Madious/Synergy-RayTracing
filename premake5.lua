
workspace "Synergy-RayTracing"
	architecture "x64"
	configurations { "Debug", "Release", "Dist" }
	flags { "MultiProcessorCompile" }
	startproject "RayTracing"



-- Output directory
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"


-- Dependencies
group "Dependencies"
	include "Dependencies/GLFW/Build.lua"
	include "Dependencies/ImGui/Build.lua"
	include "Dependencies/yaml/Build.lua"
group ""


-- Projects
include "External.lua"
include "Synergy/Build.lua"
include "RayTracing/Build.lua"
