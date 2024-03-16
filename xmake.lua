add_rules("mode.debug", "mode.release")

set_languages("cxx20")

add_requires("subprocess.h", "nlohmann_json", "vcpkg::argumentum")

local usvfs_root = os.getenv("USVFS_ROOT")
if not usvfs_root then
    print("The environment variable 'USVFS_ROOT' must be set!")
elseif not os.isdir(usvfs_root) then
    print("The environment variable 'USVFS_ROOT' does not point to a valid directory!")
end

target("usvfs-cli")
    set_kind("binary")
    set_basename("usvfs")
    add_files("main.cpp")
    add_includedirs("include")
    add_packages("subprocess.h", "nlohmann_json", "vcpkg::argumentum")
    add_includedirs(usvfs_root .. "/include")
    add_linkdirs(usvfs_root .. "/lib")
    add_links("usvfs_x64")
    add_defines("NOMINMAX")
