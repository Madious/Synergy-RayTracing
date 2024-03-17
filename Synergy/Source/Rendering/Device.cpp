/// \ project: Synergy
/// \ file: Device.cpp
/// \ author: Thomas Clermont

#include "Device.h"


namespace SNG
{

	Device::Device(Ref<PhysicalDevice> physicalDevice)
	{
		// Queue Family
		{
			uint32_t queueCount = 0;
			vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice->Raw(), &queueCount, nullptr);
			VkQueueFamilyProperties* queueFamilies = new VkQueueFamilyProperties[queueCount];
			vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice->Raw(), &queueCount, queueFamilies);

			for (uint32_t i = 0; i < queueCount; i++)
			{
				if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
				{
					m_QueueFamily = i;
					break;
				}
			}

			SNG_ASSERT(m_QueueFamily != (uint32_t)-1);
			delete[] queueFamilies;
		}

		// DEVICE
		{
			uint32_t extensionCount = 1;
			const char* extensions[] = { "VK_KHR_swapchain" };
			const float queuePriority[] = { 1.0f };

			VkDeviceQueueCreateInfo queueCreateInfo[1] = {};
			queueCreateInfo[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo[0].queueFamilyIndex = m_QueueFamily;
			queueCreateInfo[0].pQueuePriorities = queuePriority;
			queueCreateInfo[0].queueCount = 1;

			VkDeviceCreateInfo deviceInfo{};
			deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
			deviceInfo.enabledExtensionCount = extensionCount;
			deviceInfo.ppEnabledExtensionNames = extensions;
			deviceInfo.pQueueCreateInfos = queueCreateInfo;
			deviceInfo.queueCreateInfoCount = 1;

			#ifdef VULKAN_DEBUG_REPORT

				const char* layers[] = { "VK_LAYER_KHRONOS_validation" };
				deviceInfo.enabledLayerCount = 1;
				deviceInfo.ppEnabledLayerNames = layers;

			#endif

			auto err = vkCreateDevice(physicalDevice->Raw(), &deviceInfo, nullptr, &m_Device);
			check_vk_result(err, "Failed to created a device");
			vkGetDeviceQueue(m_Device, m_QueueFamily, 0, &m_Queue);
		}
	}

	Device::~Device()
	{
		vkDestroyDevice(m_Device, nullptr);
	}

}