project "bfbmp_read"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"

    targetdir(binloc)
	objdir(intloc)

    files {
        "source/main.cpp"
    }

    includedirs { 
        "%{wks.location}/bfbmp/include",
        "%{wks.location}/dependencies/tinyfiledialogs/include"
    }

    links {
     "bfbmp",
     "tinyfiledialogs"
    }

    filter "options:lib=shared"
        defines "BFBMP_SHARED"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "On"
        optimize "Off"

    filter "configurations:Release"
        runtime "Release"
        symbols "Off"
        optimize "On"