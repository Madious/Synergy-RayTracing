
project "ImGui"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    systemversion "latest"
    staticruntime "On"

    targetdir("bin/" .. outputdir .. "/%{prj.name}")
    objdir("bin-int/" .. outputdir .. "/%{prj.name}")

    includedirs
    {
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.VulkanSDK}",
    }

    files
    {
        "imconfig.h",
        "imgui.h",
        "imgui.cpp",
        "imgui_tables.cpp",
        "imgui_internal.h",
        "imgui_draw.cpp",
        "imgui_widgets.cpp",
        "imstb_rectpack.h",
        "imstb_textedit.h",
        "imstb_truetype.h",
        "imgui_demo.cpp",
        "imgui_stdlib.h",
        "imgui_stdlib.cpp",
        "imgui_impl_glfw.cpp",
        "imgui_impl_vulkan.cpp",
    }

    filter "configurations:Debug"
        buildoptions "/MTd"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        buildoptions "/MT"
        runtime "Release"
        optimize "On"

    filter "configurations:Dist"
        buildoptions "/MT"
        runtime "Release"
        optimize "On"
