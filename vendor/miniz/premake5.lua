project "miniz"
    kind "StaticLib"
    language "C"
    flags "MultiProcessorCompile"

    location(build)
	targetdir(binaries)
	objdir(intermediates)

    files {
        "source/miniz.c",
        "include/miniz/miniz.h",
        "premake5.lua"
    }

    includedirs "include"

    defines { "MINIZ_NO_STDIO", "MINIZ_NO_DEFLATE_APIS", "MINIZ_NO_INFLATE_APIS" }

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