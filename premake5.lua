workspace "RubiksCubeSolver"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

	flags
    {
        "MultiProcessorCompile"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "RubiksCubeSolver"
    location "RubiksCubeSolver"
    kind "ConsoleApp"
    language "c++"
    cppdialect "c++17"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	
	files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

	filter "system:windows"
        systemversion "latest"

        defines 
        {
            "RCS_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "RCS_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "RCS_RELEASE"
        optimize "On"

    filter "configurations:Dist"
        defines "RCS_DIST"
        optimize "On"