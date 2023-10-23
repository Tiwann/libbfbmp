workspace "libbfbmp"
    architecture "x64"
    configurations { "debug", "release", "dist" }
    platforms { "static", "shared" }
    startproject "beatmap_reading"

    build_folder = "build"
    binaries_folder = "binaries"
    intermediates_folder = "binaries-int"
    

    binaries = path.join("%{wks.location}", binaries_folder, "%{cfg.buildcfg}", "%{prj.name}_%{cfg.platform}")
    intermediates = path.join("%{wks.location}", intermediates_folder, "%{cfg.buildcfg}", "%{prj.name}_%{cfg.platform}")
    build = path.join("%{wks.location}", build_folder, "%{prj.name}")

    newaction {
        trigger = "clean",
        shortname = "clean",
        description = "Delete all generated files and compiled binaries (including IDE artifacts)",

        onStart = function()
            print("Deleting generated files...")
        end,

        execute = function()
            os.rmdir(binaries_folder)
            os.rmdir(intermediates_folder)
            os.rmdir(".idea")
            os.rmdir(".vs")
            os.rmdir(".vscode")
            os.rmdir(build_folder)
            os.remove("*.sln")
            os.remove("Makefile")
        end,

        onEnd = function()
            print("Done.")
        end
    }

    include "bfbmp"
    include "vendor/miniz"
    include "examples/beatmap_reading"
    
    
