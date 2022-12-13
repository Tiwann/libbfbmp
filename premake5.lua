workspace "libbfbmp"
	architecture "x64"
	startproject "reading"
	configurations { "Debug", "Release" }

group "examples"
project "writing"
	location "examples/writing"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	debugdir "%{prj.location}"
	targetdir "%{prj.location}/binaries/%{cfg.buildcfg}"
	objdir "%{prj.location}/intermediate/%{cfg.buildcfg}"

	files {
		"%{wks.location}/include/bfbmp.h",
		"%{prj.location}/main.cpp",
	}

	includedirs {
		"%{wks.location}/include",
		"%{wks.location}/dependencies/tinyfiledialogs/include",
	}
	
	links "tinyfiledialogs"

	filter "system:windows"
		defines "_CRT_SECURE_NO_WARNINGS"
	
	filter "configurations:Debug"
		symbols "On"
		optimize "Off"
		runtime "Debug"
			
	filter "configurations:Release"
		symbols "Off"
		optimize "On"
		runtime "Release"
	
project "reading"
	location "examples/reading"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	debugdir "%{prj.location}"
	targetdir "%{prj.location}/binaries/%{cfg.buildcfg}"
	objdir "%{prj.location}/intermediate/%{cfg.buildcfg}"

	files {
		"%{wks.location}/include/bfbmp.h",
		"%{prj.location}/main.cpp",
	}
	
	links "tinyfiledialogs"

	includedirs {
		"%{wks.location}/include",
		"%{wks.location}/dependencies/tinyfiledialogs/include",
	}

	filter "system:windows"
		defines "_CRT_SECURE_NO_WARNINGS"
	
	filter "configurations:Debug"
			symbols "On"
			optimize "Off"
			runtime "Debug"
			
		filter "configurations:Release"
			symbols "Off"
			optimize "On"
			runtime "Release"

project "dllbuild"
location "examples/dllbuild"
	kind "SharedLib"
	language "C++"
	cppdialect "C++20"
	debugdir "%{prj.location}"
	targetdir "%{prj.location}/binaries/%{cfg.buildcfg}"
	objdir "%{prj.location}/intermediate/%{cfg.buildcfg}"

	files {
		"%{prj.location}/main.cpp",
		"%{wks.location}/include/bfbmp.h"
	}

	includedirs {
		"%{wks.location}/include",
	}

	defines {
		"BFBMP_BUILD_DLL",
		"BFBMP_EXPORT",
	}


	filter "system:windows"
		defines "_CRT_SECURE_NO_WARNINGS"
	
	filter "configurations:Debug"
		symbols "On"
		optimize "Off"
		runtime "Debug"
			
	filter "configurations:Release"
		symbols "Off"
		optimize "On"
		runtime "Release"



group "dependencies"
project "tinyfiledialogs"
	location "dependencies/tinyfiledialogs"
	kind "StaticLib"
	language "C"
	debugdir "%{prj.location}"
	targetdir "%{prj.location}/binaries/%{cfg.buildcfg}"
	objdir "%{prj.location}/intermediate/%{cfg.buildcfg}"

	files {
		"%{prj.location}/include/tinyfiledialogs.h",
		"%{prj.location}/src/tinyfiledialogs.c",
	}
	
	includedirs {
		"%{prj.location}/include",
	}
	
	filter "system:windows"
		defines "_CRT_SECURE_NO_WARNINGS"
	
	filter "configurations:Debug"
		symbols "On"
		optimize "Off"
		runtime "Debug"
		
	filter "configurations:Release"
		symbols "Off"
		optimize "On"
		runtime "Release"
	
	filter "system:windows"
		links { "comdlg32.lib", "ole32.lib" }


group "scripts"
project "scripts"
	kind "None"
	
	files {
		"%{wks.location}/premake5.lua",
		"%{wks.location}/build.bat",
	}

newaction {
	trigger = "clean",
	description = "clean all generated files",
	execute = function()
		folders = os.matchdirs("**/binaries")
		for i in pairs(folders) do
			os.rmdir(folders[i])
		end

		folders = os.matchdirs("**/intermediate")
		for i in pairs(folders) do
			os.rmdir(folders[i])
		end

		os.rmdir(".idea")
		os.rmdir(".vs")
		os.rmdir(".vscode")
		os.remove("*.sln")
		os.remove("**.vcxproj")
		os.remove("**.vcxproj.filters")
		os.remove("**.vcxproj.user")
		os.remove("**.make")
		os.remove("**Makefile")
		os.remove("**.cmake")
		os.remove("**.CMakeLists.txt")
	end

}
