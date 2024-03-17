/// \ project: Synergy-RayTracing
/// \ file: Core.h
/// \ author: Thomas Clermont

#pragma once


#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

#include <glm/glm.hpp>

#include <iostream>
#include <algorithm>
#include <execution>
#include <functional>
#include <random>
#include <vector>
#include <stdint.h>



#ifdef SNG_DEBUG
	#define VULKAN_DEBUG_REPORT
	#define SNG_ASSERT(x) if (!(x)) __debugbreak();
#else
	#define SNG_ASSERT(x) x;
#endif



#define MAX_IMAGE_COUNT 3


// Functions
namespace SNG
{

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename... Args>
	Scope<T> MakeScope(Args... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T, typename... Args>
	Ref<T> MakeRef(Args... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}




	void check_vk_result(VkResult result, const char* message = "");

	VkCommandBuffer OneTimeCommandBuffer();
	void EndOneTimeCommandBuffer(VkCommandBuffer commandBuffer);

	VKAPI_ATTR VkBool32 VKAPI_CALL VulkanErrorCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallback,
		void* pUserData);

}
