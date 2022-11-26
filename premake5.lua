workspace "libbfbmp"
    architecture "x64"
    startproject "reading"
    configurations { "Debug", "Release" }

group "examples"
project "writing"
    location "examples/writing"
    kind "ConsoleApp"
    language "C"
    cdialect "C17"
    debugdir "%{prj.location}"
    targetdir "%{prj.location}/binaries"
    objdir "%{prj.location}/intermediate"

    files {
        "%{wks.location}/include/bfbmp.h",
        "%{prj.location}/writing_main.c",
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
    language "C"
    cdialect "C17"
    debugdir "%{prj.location}"
    targetdir "%{prj.location}/binaries"
    objdir "%{prj.location}/intermediate"

    files {
        "%{wks.location}/include/bfbmp.h",
        "%{prj.location}/reading_main.c",
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

group "dependencies"
project "tinyfiledialogs"
    location "dependencies/tinyfiledialogs"
    kind "StaticLib"
    language "C"
    debugdir "%{prj.location}"
    targetdir "%{prj.location}/binaries"
    objdir "%{prj.location}/intermediate"

    files {
        "%{prj.location}/include/tfd.h",
        "%{prj.location}/src/tfd.c",
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
        links { "Comdlg32.lib", "Ole32.lib" }


group "scripts"
project "scripts"
    kind "None"
    
    files {
        "%{wks.location}/premake5.lua",
        "%{wks.location}/build.bat",
    }
