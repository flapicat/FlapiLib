workspace "FlapiLib"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

LibDir = {}
LibDir["Assimp"] = "%{wks.location}/FlapiLib/vendor/assimp/lib"

IncludeDir = {}
IncludeDir["GLFW"] = "FlapiLib/vendor/GLFW/include"
IncludeDir["glad"] = "FlapiLib/vendor/glad/include"
IncludeDir["ImGui"] = "FlapiLib/vendor/imgui"
IncludeDir["glm"] = "FlapiLib/vendor/glm"
IncludeDir["stb"] = "FlapiLib/vendor/stb"
IncludeDir["Assimp"] = "FlapiLib/vendor/assimp/include"

group "Dependencies"
	include "FlapiLib/vendor/GLFW"
	include "FlapiLib/vendor/glad"
	include "FlapiLib/vendor/imgui"
group ""

project "FlapiLib"
	location "FlapiLib"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	buildoptions "/utf-8"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "pch.h"
	pchsource "FlapiLib/src/pch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.ImGui}/backends",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb}",
		"%{IncludeDir.Assimp}"
	}
	
	links 
	{ 
		"GLFW",
		"glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines "DEBUG"
		runtime "Debug"
		symbols "on"
		links { 
			"%{LibDir.Assimp}/Debug/assimp-vc143-mtd.lib"
		}


	filter "configurations:Release"
		defines "RELEASE"
		runtime "Release"
		optimize "on"
		links { 
			"%{LibDir.Assimp}/Release/assimp-vc143-mt.lib"
		}

		
project "Examples"
	location "Examples"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
    buildoptions { "/utf-8" }

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	
	files{
		"%{prj.name}/Assets/**",
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs{
		"FlapiLib/vendor/spdlog/include",
		"FlapiLib/src",
		"FlapiLib/vendor",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.ImGui}/backends",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb}"
	}

	links{
		"FlapiLib",
	} 


	postbuildcommands {
		'{COPYDIR} "%{prj.location}/Assets" "%{cfg.targetdir}/Assets"'
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "RELEASE"
		runtime "Release"
		optimize "on"