workspace "FlapiLib"
	architecture "x64"
    startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

LibDir = {}
LibDir["Assimp"] = "%{wks.location}/FlapiLib/vendor/assimp/lib"
LibDir["irrKlang"] = "%{wks.location}/FlapiLib/vendor/irrKlang/lib"

IncludeDir = {}
IncludeDir["GLFW"] = "FlapiLib/vendor/GLFW/include"
IncludeDir["glad"] = "FlapiLib/vendor/glad/include"
IncludeDir["ImGui"] = "FlapiLib/vendor/imgui"
IncludeDir["glm"] = "FlapiLib/vendor/glm"
IncludeDir["stb"] = "FlapiLib/vendor/stb"
IncludeDir["Assimp"] = "FlapiLib/vendor/assimp/include"
IncludeDir["irrKlang"] = "FlapiLib/vendor/irrKlang/include"

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
		"%{prj.name}/vendor/stb/**"
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
		"%{IncludeDir.Assimp}",
		"%{IncludeDir.irrKlang}"
	}

	libdirs {
        "%{LibDir.Assimp}",
        "%{LibDir.irrKlang}/Winx64"
    }

	links 
	{ 
		"GLFW",
		"glad",
		"ImGui",
		"irrKlang",
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
			"C:/dev/FlapiLib/FlapiLib/vendor/assimp/lib/Debug/assimp-vc143-mtd.lib"
		}


	filter "configurations:Release"
		defines "RELEASE"
		runtime "Release"
		optimize "on"
		links { 
			"C:/dev/FlapiLib/FlapiLib/vendor/assimp/lib/Release/assimp-vc143-mt.lib"
		}

		
project "Sandbox"
	location "Sandbox"
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
		"%{IncludeDir.stb}",
		"%{IncludeDir.Assimp}"
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

		
project "Pong"
	location "Pong"
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
		"%{IncludeDir.stb}",
		"%{IncludeDir.Assimp}",
		"%{IncludeDir.irrKlang}"
	}

	links{
		"FlapiLib",
	} 


	filter "system:windows"
		systemversion "latest"
		
	postbuildcommands {
		'if exist "%{prj.location}/Assets" xcopy /Q /E /Y /I "%{prj.location}/Assets" "%{cfg.targetdir}/Assets" > nul',

		'if exist "%{wks.location}/FlapiLib/vendor/irrKlang/bin/irrKlang.dll" copy /Y "%{wks.location}/FlapiLib/vendor/irrKlang/bin/irrKlang.dll" "%{cfg.targetdir}" > nul'
	}

	filter "configurations:Debug"
		defines "DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "RELEASE"
		runtime "Release"
		optimize "on"

		
project "AABBCollisionTest"
	location "AABBCollisionTest"
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
		"%{IncludeDir.stb}",
		"%{IncludeDir.Assimp}"
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