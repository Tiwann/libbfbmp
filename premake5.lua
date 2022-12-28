workspace "libbfbmp"
    architecture "x64"
    configurations { "Debug", "Release" }
    flags "MultiProcessorCompile"

    binloc = "%{wks.location}/build/binaries/%{cfg.buildcfg}"
    intloc = "%{wks.location}/build/intermediate/%{cfg.buildcfg}"

    include "premake/clean.lua"
    include "premake/options.lua"
    include "bfbmp/bfbmp.lua"
    include "dependencies/tinyfiledialogs/tinyfiledialogs.lua"
    include "examples/bfbmp_read/bfbmp_read.lua"
    include "scripts.lua"