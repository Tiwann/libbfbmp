project "bfbmp"
	language "C++"
	cppdialect "C++20"

	targetdir(binloc)
	objdir(intloc)
	
	files {
		"source/**.h",
		"source/**.cpp",
		"include/**.h"
	}

	includedirs {
		"include"
	}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"
		optimize "Off"

	filter "configurations:Release"
		runtime "Release"
		symbols "Off"
		optimize "On"

	filter "options:lib=shared"
		kind "SharedLib"
		defines "BFBMP_SHARED"
		defines "BFBMP_EXPORTS"
		staticruntime "Off"
	
	filter "options:lib=static"
		kind "StaticLib"
		defines "BFBMP_STATIC"
		staticruntime "On"



		