
-- Libraries include paths
VULKAN_SDK = os.getenv("VULKAN_SDK")

IncludeDir = {}
IncludeDir["VulkanSDK"] = "%{VULKAN_SDK}/Include"
IncludeDir["GLFW"] = "%{wks.location}/Dependencies/GLFW/include"
IncludeDir["ImGui"] = "%{wks.location}/Dependencies/ImGui"
IncludeDir["stb_image"] = "%{wks.location}/Dependencies/stb_image"
IncludeDir["yaml"] = "%{wks.location}/Dependencies/yaml/include"


-- Libraries '.lib' paths
LibDir = {}
LibDir["VulkanSDK"] = "%{VULKAN_SDK}/Lib"


