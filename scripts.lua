project "scripts"
    kind "None"
    
    targetdir(binloc)
    objdir(intloc)
    
    files {
        "premake5.lua",
        "bfbmp/bfbmp.lua",
        "examples/bfbmp_read/bfbmp_read.lua",
        "dependencies/tinyfiledialogs/tinyfiledialogs.lua"
    }