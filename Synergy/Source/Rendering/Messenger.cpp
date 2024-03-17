/// \ project: Synergy
/// \ file: Messenger.cpp
/// \ author: Thomas Clermont

#include "Messenger.h"


namespace SNG
{

	static VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
		const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkDebugUtilsMessengerEXT* pMessenger)
	{
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
		if (func) return func(instance, pCreateInfo, pAllocator, pMessenger);
		else return VK_ERROR_EXTENSION_NOT_PRESENT;
	}

	static void DestroyDebugUtilsMessengerEXT(VkInstance instance,
		VkDebugUtilsMessengerEXT messenger,
		const VkAllocationCallbacks* pAllocator)
	{
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func) func(instance, messenger, pAllocator);
	}




	Messenger::Messenger(Ref<Instance> instance)
		: r_Instance(instance)
	{
		#ifdef VULKAN_DEBUG_REPORT

			VkDebugUtilsMessengerCreateInfoEXT messengerCreateInfo{};
			messengerCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
			messengerCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
				VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
			messengerCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
				VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
			messengerCreateInfo.pfnUserCallback = VulkanErrorCallback;
			messengerCreateInfo.pUserData = nullptr;

			VkResult err = CreateDebugUtilsMessengerEXT(r_Instance->Raw(),
				&messengerCreateInfo, nullptr, &m_Messenger);
			check_vk_result(err, "Failed to created a messenger");

		#endif
	}

	Messenger::~Messenger()
	{
		#ifdef VULKAN_DEBUG_REPORT
			DestroyDebugUtilsMessengerEXT(r_Instance->Raw(), m_Messenger, nullptr);
		#endif
	}

}
