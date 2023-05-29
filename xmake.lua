-- add compilation definitons
set_project("networkssy")
set_description("A simple network library and applications for C++")
set_languages("c++20")
set_version("0.1.0")
set_license("MIT")
-- set_homepages("https://git.bry.com.br/app")
-- set_urls(s"https://git.bry.com.br/app.git")
set_xmakever("2.7.9")

-- settings
add_rules("mode.debug", "mode.release")
set_warnings("all", "error")
set_optimize("fastest")
add_includedirs("include", "/usr/local/include", "/usr/include")
add_cxxflags("-std=c++20 -Wall -Wextra -Werror -Wpedantic", { force = true })
-- add_asflags("")
-- add_ldflags("-L/usr/local/lib", "-L/usr/lib")
-- add_shflags("")
-- add_arflags("")
set_policy("build.optimization.lto", true)
set_policy("build.warning", true)
set_warnings("allextra")
set_policy("build.merge_archive", true)
set_policy("package.requires_lock", true)

-- platform specific settings
if is_plat("linux") then
	set_toolchains("clang")
elseif is_plat("macosx") then
	set_toolchains("clang")
elseif is_plat("windows") then
	set_toolchains("msvc")
end

-- repositories
-- add_repositories("bry https://git.bry.com.br/cpp/repo.git main")

-- dependencies
local networkssy_deps = { "spdlog", "fmt" }
local clientssy_deps = { "fmt", "cxxopts", "spdlog" }
local serverssy_deps = { "fmt", "cxxopts", "spdlog" }
local test_deps = { "gtest" }
local bench_deps = { "benchmark" }

add_requires(table.unpack(networkssy_deps))
add_requires(table.unpack(clientssy_deps))
add_requires(table.unpack(serverssy_deps))
add_requires(table.unpack(test_deps))
add_requires(table.unpack(bench_deps))

-- the main library target
target("networkssy", function()
	-- set_default(true)
	set_kind("shared")
	add_files("src/networkssy/*.cpp")
	add_packages(table.unpack(networkssy_deps))
	set_installdir("/usr/local")
end)

-- client target
target("clientssy", function()
	set_default(true)
	set_kind("binary")
	add_files("src/clientssy/*.cpp")
	add_packages(table.unpack(clientssy_deps))
	add_deps("networkssy")
	set_installdir("/usr/local")
end)

--  server target
target("serverssy", function()
	set_default(true)
	set_kind("binary")
	add_files("src/serverssy/*.cpp")
	add_packages(table.unpack(serverssy_deps))
	add_deps("networkssy")
	set_installdir("/usr/local")
end)

-- test suites for all targets
target("test", function()
	set_default(false)
	set_kind("binary")
	add_files("test/**/*.cpp", "test/main.cpp")
	add_packages(table.unpack(test_deps))
	add_deps("networkssy")
	set_installdir("/usr/local")
end)

-- benchmarks for all targets
target("bench", function()
	set_default(false)
	set_kind("binary")
	add_files("bench/**/*.cpp", "bench/main.cpp")
	add_packages(table.unpack(bench_deps))
	add_deps("networkssy")
	set_installdir("/usr/local")
end)
