/// \ project: Synergy
/// \ file: Instance.cpp
/// \ author: Thomas Clermont

#include "Instance.h"


namespace SNG
{

	Instance::Instance()
	{
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		VkInstanceCreateInfo instanceInfo{};
		instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

		#ifdef VULKAN_DEBUG_REPORT

			VkDebugUtilsMessengerCreateInfoEXT messengerCreateInfo{};
			messengerCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
			messengerCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
				VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
			messengerCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
				VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
			messengerCreateInfo.pfnUserCallback = VulkanErrorCallback;
			messengerCreateInfo.pUserData = nullptr;

			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
			instanceInfo.enabledExtensionCount = (uint32_t)extensions.size();
			instanceInfo.ppEnabledExtensionNames = extensions.data();
			const char* layers[] = { "VK_LAYER_KHRONOS_validation" };
			instanceInfo.enabledLayerCount = 1;
			instanceInfo.ppEnabledLayerNames = layers;
			instanceInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&messengerCreateInfo;

			VkResult err = vkCreateInstance(&instanceInfo, nullptr, &m_Instance);
			check_vk_result(err, "Failed to created an instance");

		#else

			instanceInfo.enabledExtensionCount = (uint32_t)extensions.size();
			instanceInfo.ppEnabledExtensionNames = extensions.data();
			instanceInfo.enabledLayerCount = 0;
			VkResult err = vkCreateInstance(&instanceInfo, nullptr, &m_Instance);
			check_vk_result(err, "Failed to created an instance");

		#endif
	}

	Instance::~Instance()
	{
		vkDestroyInstance(m_Instance, nullptr);
	}

}