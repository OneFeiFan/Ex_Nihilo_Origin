add_rules("mode.debug", "mode.release")

add_repositories("levimc-repo https://github.com/LiteLDev/xmake-repo.git")

option("target_type")
    set_default("server")
    set_showmenu(true)
    set_values("server", "client")
option_end()

-- add_requires("levilamina x.x.x") for a specific version
-- add_requires("levilamina develop") to use develop version
-- please note that you should add bdslibrary yourself if using dev version
add_requires("levilamina", {configs = {target_type = get_config("target_type")}})

add_requires("levibuildscript")

if not has_config("vs_runtime") then
    set_runtimes("MD")
end

add_rules("plugin.compile_commands.autoupdate", {outputdir = "."})

target("Ex_Nihilo_Origin") -- Change this to your mod name.
    add_defines("NAME_SPACE=\"Ex_Nihilo_Origin\"")
    add_rules("@levibuildscript/linkrule")
    add_rules("@levibuildscript/modpacker")
    add_cxflags( "/EHa", "/utf-8", "/W4", "/w44265", "/w44289", "/w44296", "/w45263", "/w44738", "/w45204")
    add_defines("NOMINMAX", "UNICODE")
    add_packages("levilamina")
    set_exceptions("none") -- To avoid conflicts with /EHa.
    set_kind("shared")
    set_languages("c++20")
    set_symbols("debug")
    add_headerfiles("src/**.h")
    add_files("src/**.cpp")
    add_includedirs("src")
    if is_config("target_type", "server") then
        add_defines("LL_PLAT_S")
    --  add_includedirs("src-server")
    --  add_files("src-server/**.cpp")
    else
        add_defines("LL_PLAT_C")
    --  add_includedirs("src-client")
    --  add_files("src-client/**.cpp")
    end

on_load(function (target)
        import("core.base.json")
        local tag = os.iorun("git describe --tags --abbrev=0 --always")
        local major, minor, patch, suffix = tag:match("v(%d+)%.(%d+)%.(%d+)(.*)")
        if not major then
            print("Failed to parse version tag, using version from tooth.json")
            tag = json.loadfile("tooth.json")["version"]
            major, minor, patch, suffix = tag:match("(%d+)%.(%d+)%.(%d+)(.*)")
        end
        local versionStr =  major.."."..minor.."."..patch
        if suffix then
            prerelease = suffix:match("-(.*)")
            if prerelease then
                prerelease = prerelease:gsub("\n", "")
            end
            if prerelease then
                target:set("configvar", "LL_VERSION_PRERELEASE", prerelease)
                versionStr = versionStr.."-"..prerelease
            end
        end
        target:set("configvar", "LL_VERSION_MAJOR", major)
        target:set("configvar", "LL_VERSION_MINOR", minor)
        target:set("configvar", "LL_VERSION_PATCH", patch)

        if not has_config("publish") then
            local hash = os.iorun("git rev-parse --short HEAD")
            versionStr = versionStr.."+"..hash:gsub("\n", "")
        end

        target:add("rules", "@levibuildscript/modpacker",{
               modVersion = versionStr
           })
    end)