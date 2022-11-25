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
        "%{prj.location}/main.c",
    }

    includedirs {
        "%{wks.location}/include"
    }

    filter "system:windows"
        defines "_CRT_SECURE_NO_WARNINGS"
    filter ""
    
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
        "%{prj.location}/main.c",
    }

    includedirs {
        "%{wks.location}/include"
    }

    filter "system:windows"
        defines "_CRT_SECURE_NO_WARNINGS"
    filter ""
    
group "scripts"
project "scripts"
    kind "None"
    
    files {
        "%{wks.location}/premake5.lua",
        "%{wks.location}/build.bat",
    }
