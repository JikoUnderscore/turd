workspace "turdproj"
    architecture "x64"
    configurations { "Debug", "Release" }
    startproject "turdproj"

project "turdproj"
    location "turdproj"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"

    warnings "High"

    targetdir ("%{wks.location}/target/dis.%{prj.name}/%{cfg.architecture}/%{cfg.buildcfg}")
    objdir ("%{wks.location}/target/build.%{prj.name}/%{cfg.architecture}/%{cfg.buildcfg}")

    
    files {
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.hpp",
        "%{prj.name}/src/**.cppm",
        "%{prj.name}/src/**.ixx",
        "premake5.lua",
    }

    includedirs {
        "/home/liko/repos/turd/vendor/SDL2-2.24.0/include",
        "/home/liko/repos/turd/vendor/SDL_image/include",
    }

    links{
        "SDL2",
        "SDL2main",
        "SDL2_image",
    }

    libdirs {
       "/usr/local/lib"
    }

    filter "configurations:Debug"
        buildoptions "-std=c++20 -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -Wold-style-cast -Wcast-align -Wunused -Woverloaded-virtual -Wpedantic -Wconversion -Wsign-conversion -Wmisleading-indentation -Wduplicated-cond -Wduplicated-branches -Wlong-long -Wnull-dereference -Wuseless-cast -Wdouble-promotion -Wformat=2 -Wdate-time"
        
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        buildoptions "-std=c++20 -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -Wold-style-cast -Wcast-align -Wunused -Woverloaded-virtual -Wpedantic -Wconversion -Wsign-conversion -Wmisleading-indentation -Wduplicated-cond -Wduplicated-branches -Wlogical-op -Wnull-dereference -Wuseless-cast -Wdouble-promotion -Wformat=2 -Wdate-time"
        defines { "NDEBUG" }
        -- optimize "On"
        optimize "Speed"