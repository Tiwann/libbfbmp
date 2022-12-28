group "dependencies"
project "tinyfiledialogs"
    kind "StaticLib"
    language "C"
    staticruntime "On"
    
    targetdir(binloc)
    objdir(intloc)
    
    files {
        "source/tinyfiledialogs.c",
        "include/tinyfiledialogs.h"
    }
    
    includedirs "%{prj.location}/include"
    
    filter "configurations:Debug"
    	runtime "Debug"
    	symbols "On"
    	optimize "Off"
    
    filter "configurations:Release"
    	runtime "Release"
    	symbols "Off"
    	optimize "On"
group ""