project "bfbmp"
	language "C"
	cdialect "C17"
	flags "MultiProcessorCompile"
	
	location(build)
	targetdir(binaries)
	objdir(intermediates)


	files {
		"src/*.c",
		"src/*.h",
		"include/**.h",
		"include/**.hpp",
		"premake5.lua"
	}

	includedirs {
		"include",
		"src",
		"%{wks.location}/vendor/miniz/include"
	}
	
	defines {  }
	
	links { 
	    "miniz"
	}
	
	filter "Platforms:static"
	    kind "StaticLib"
	    defines "BFBMP_STATIC"
	    
	filter "Platforms:shared"
    	kind "SharedLib"
    	defines { "BFBMP_SHARED", "BFBMP_SHARED_EXPORT" }
    	postbuildcommands { 
    	    "{COPY} %{cfg.buildtarget.relpath} %{wks.location}/binaries/%{cfg.buildcfg}/beatmap_reading_shared"
    	}
	    
	filter "Configurations:debug"
		runtime "Debug"
		symbols "On"
		optimize "Off"

	filter "Configurations:release"
		runtime "Release"
		symbols "On"
		optimize "On"

	filter "Configurations:dist"
		runtime "Release"
		symbols "Off"
		optimize "On"
		
	filter "System:Windows"
		defines "_CRT_SECURE_NO_WARNINGS"
		targetname "libbfbmp"
		
	filter "System:not Windows"
	    targetname "bfbmp"