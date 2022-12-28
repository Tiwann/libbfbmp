newaction {
    trigger = "clean",
    description = "Clean all the generated files",
    
    onStart = function()
        print("Cleaning all the generated files...")
    end,

    execute = function()
        os.rmdir("build")
        os.rmdir(".vs")
        os.rmdir(".idea")
        os.remove("**.vcxproj")
        os.remove("**.vcxproj.filters")
        os.remove("**.vcxproj.user")
        os.remove("**.sln")
        os.remove("**.make")
        os.remove("**Makefile")
    end,

    onEnd = function()
        print("Done.")
    end
}