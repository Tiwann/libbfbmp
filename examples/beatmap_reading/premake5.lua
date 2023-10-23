project "beatmap_reading"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    flags "MultiProcessorCompile"
    
    
    location(build)
    targetdir(binaries)
    objdir(intermediates)
    debugdir "%{wks.location}/examples/beatmap_reading"
    

    files {
        "src/*.cpp",
        "src/*.h",
        "premake5.lua"
    }

    externalincludedirs {
        "%{wks.location}/bfbmp/include",
    }
    
    links { "bfbmp" }
    
    filter "Platforms:static"
        defines "BFBMP_STATIC"
    	    
    filter "Platforms:shared"
        defines "BFBMP_SHARED"
       
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
        