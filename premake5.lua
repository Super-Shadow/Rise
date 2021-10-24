workspace "Rise"
	architecture "x86_64"
	startproject "Sandbox" 

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

--Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Rise/vendor/GLFW/include"
IncludeDir["Glad"] = "Rise/vendor/Glad/include"
IncludeDir["ImGui"] = "Rise/vendor/imgui"
IncludeDir["glm"] = "Rise/vendor/glm"
IncludeDir["stb_image"] = "Rise/vendor/stb_image"

group "Dependencies"
	include "Rise/vendor/GLFW"
	include "Rise/vendor/Glad"
	include "Rise/vendor/imgui"

group ""

project "Rise"
	location "Rise"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"


	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "rspch.h"
	pchsource "Rise/src/rspch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}

	includedirs
	{
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/src",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"
		
		defines
		{
			"RS_BUILD_DLL",
			"GLFW_INCLUDE_NONE",
			"_CRT_SECURE_NO_WARNINGS"
		}
		filter "toolset:msc"
			disablewarnings "4481"


	filter "configurations:Debug"
		defines "RS_DEBUG"
		filter "toolset:msc*"
			disablewarnings "4481"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "RS_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "RS_DIST"
		runtime "Release"
		optimize "on"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Rise/vendor/spdlog/include",
		"Rise/src",
		"Rise/vendor",
		"%{IncludeDir.glm}"
	}

	links
	{
		"Rise"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "RS_DEBUG"
		filter "toolset:msc*"
			disablewarnings "4481"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "RS_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "RS_DIST"
		runtime "Release"
		optimize "on"
