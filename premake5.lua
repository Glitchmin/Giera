workspace "Giera"
	architecture "x86_64"
	startproject "Giera"

	configurations
	{
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Giera"
	location "Giera"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"

	targetdir ("bin/" .. outputdir .. "")
	objdir ("bin/" .. outputdir .. "")

	files
	{
		"%{prj.name}/**.h",
		"%{prj.name}/**.cpp"
	}

	includedirs
	{
		"%{prj.name}",
		"SDL/SDL2-2.30.1/include",
		"SDL/SDL2_image-2.8.2/include",
		"SDL/SDL2_ttf-2.22.0/include"
	}

	libdirs
	{
		"SDL/SDL2-2.30.1/lib/x64",
		"SDL/SDL2_image-2.8.2/lib/x64",
		"SDL/SDL2_ttf-2.22.0/lib/x64"
	}

	links
	{
		"SDL2.lib",
		"SDL2main.lib",
		"SDL2_image.lib",
		"SDL2_ttf.lib"
	}

	filter "system:windows"
		systemversion "latest"

		postbuildcommands
		{
			("{COPY} %{wks.location}SDL/SDL2-2.30.1/lib/x64/SDL2.dll %{wks.location}bin/" .. outputdir .. ""),
			("{COPY} %{wks.location}SDL/SDL2_image-2.8.2/lib/x64/SDL2_image.dll %{wks.location}bin/" .. outputdir .. ""),
			("{COPY} %{wks.location}SDL/SDL2_ttf-2.22.0/lib/x64/SDL2_ttf.dll %{wks.location}bin/" .. outputdir .. ""),
		}

	filter "configurations:Debug"
		symbols "On"
		runtime "Debug"
		debugdir ("%{wks.location}bin/" .. outputdir .. "")

	filter "configurations:Release"
		optimize "On"
		runtime "Release"