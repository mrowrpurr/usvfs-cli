local usvfs_root = os.getenv("USVFS_ROOT")

if not usvfs_root then
    print("The environment variable 'USVFS_ROOT' must be set!")
elseif not os.isdir(usvfs_root) then
    print("The environment variable 'USVFS_ROOT' does not point to a valid directory!")
else
    print("Using USVFS_ROOT: " .. usvfs_root)
end

-- Note: using Boost 1.79.0, precompiled binaries for Windows
local boost_root = os.getenv("BOOST_ROOT")

if not boost_root then
    print("The environment variable 'BOOST_ROOT' must be set!")
elseif not os.isdir(boost_root) then
    print("The environment variable 'BOOST_ROOT' does not point to a valid directory!")
else
    print("Using BOOST_ROOT: " .. boost_root)
end

add_rules("mode.debug", "mode.release")

-- TODO: try C++20, usvfs built using C++17 (and we're using some string functions from it)
set_languages("cxx17")

target("usvfs-cli")
    set_kind("binary")

    -- usvfs.exe
    set_basename("usvfs")
    
    add_files("usvfs-cli.cpp")

    -- usvfs
    add_includedirs(usvfs_root .. "/include")
    add_linkdirs(usvfs_root .. "/lib")
    add_links("usvfs_x64")

    -- boost (not actually needed, it's more important that usvfs is built using this!)
    -- only included in this project to verify that boost::filesystem::path works
    add_includedirs(boost_root)
    add_linkdirs(boost_root .. "/lib64-msvc-14.3")
    add_links(
        "boost_filesystem-vc143-mt-gd-x64-1_79"
    )

    -- TODO: might not need all of these, test without some of them
    add_defines("_UNICODE", "UNICODE", "BOOST_DISABLE_ASSERTS", "_CRT_SECURE_NO_WARNINGS", "NDEBUG")
