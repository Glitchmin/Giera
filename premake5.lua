-- Visual Studio 2022 solution for Windows (alternative to the CMake / VS Code build).
--
--   generateProjects.bat            -> Giera.sln (+ project files in build/premake/)
--   tools/build_premake.ps1 -Test   -> generate, build with MSBuild, run the unit tests
--
-- Uses the prebuilt SDL2 "VC" development packages in SDL2/ (git-ignored), downloaded by
--   powershell -ExecutionPolicy Bypass -File tools/fetch_deps.ps1 -Premake
-- Any SDL2-2.x / SDL2_image-2.x / SDL2_ttf-2.x folder found there is used.
--
-- Android is built from the same src/ by CMake + Gradle (see android/ and Readme.md).

local function findSdlPackage(pattern, name)
	local dirs = os.matchdirs("SDL2/" .. pattern)
	table.sort(dirs)
	local dir = dirs[#dirs]
	if dir == nil then
		premake.error(name .. " not found in SDL2/. Run: powershell -ExecutionPolicy Bypass -File tools/fetch_deps.ps1 -Premake")
	end
	return dir
end

local sdl      = findSdlPackage("SDL2-2.*", "SDL2")
local sdlImage = findSdlPackage("SDL2_image-2.*", "SDL2_image")
local sdlTtf   = findSdlPackage("SDL2_ttf-2.*", "SDL2_ttf")

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

workspace "Giera"
	architecture "x86_64"
	startproject "Giera"
	location "."

	configurations
	{
		"Debug",
		"Release"
	}

	-- settings shared by every project (same as the CMake build)
	language "C++"
	cppdialect "C++20"
	systemversion "latest"
	flags { "MultiProcessorCompile" }
	buildoptions { "/utf-8", "/permissive-", "/Zc:__cplusplus" }
	defines { "NOMINMAX", "_CRT_SECURE_NO_WARNINGS" }

	includedirs
	{
		"src",
		sdl .. "/include",
		sdlImage .. "/include",
		sdlTtf .. "/include"
	}

	libdirs
	{
		sdl .. "/lib/x64",
		sdlImage .. "/lib/x64",
		sdlTtf .. "/lib/x64"
	}

	targetdir ("bin/" .. outputdir)
	objdir ("build/premake/obj/" .. outputdir .. "/%{prj.name}")

	filter "configurations:Debug"
		symbols "On"
		runtime "Debug"
		defines { "_DEBUG" }

	filter "configurations:Release"
		optimize "On"
		symbols "On"
		runtime "Release"
		defines { "NDEBUG" }

	filter {}

-- The game puts SDL DLLs and assets/ next to the executable
-- (main() changes the working directory to the executable folder).
-- (%[...] paths are relative to this script and get translated for the shell.)
local function deployRuntime()
	local bindir = "bin/" .. outputdir
	postbuildcommands
	{
		"{COPYFILE} %[" .. sdl .. "/lib/x64/SDL2.dll] %[" .. bindir .. "]",
		"{COPYFILE} %[" .. sdlImage .. "/lib/x64/SDL2_image.dll] %[" .. bindir .. "]",
		"{COPYFILE} %[" .. sdlTtf .. "/lib/x64/SDL2_ttf.dll] %[" .. bindir .. "]",
		"{COPYDIR} %[assets] %[" .. bindir .. "/assets]"
	}
	debugdir ("bin/" .. outputdir)
end

local function linkGame()
	links
	{
		"GieraCore",
		"SDL2.lib",
		"SDL2main.lib",
		"SDL2_image.lib",
		"SDL2_ttf.lib"
	}
end

-- All game code except main(): shared by the game and the unit tests.
project "GieraCore"
	location "build/premake"
	kind "StaticLib"

	files
	{
		"src/**.h",
		"src/**.cpp"
	}
	removefiles { "src/main.cpp" }

project "Giera"
	location "build/premake"
	kind "ConsoleApp"

	files { "src/main.cpp" }
	linkGame()
	deployRuntime()

-- Unit tests + headless game-loop tests (tests/framework is a tiny test runner).
--   GieraTests.exe                  run everything
--   GieraTests.exe --file TestCombat   one file
--   GieraTests.exe --list           list test cases
project "GieraTests"
	location "build/premake"
	kind "ConsoleApp"

	files
	{
		"tests/**.h",
		"tests/**.cpp"
	}
	includedirs { "tests/framework" }
	linkGame()
	deployRuntime()
