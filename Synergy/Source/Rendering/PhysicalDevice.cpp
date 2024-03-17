/// \ project: Synergy
/// \ file: PhysicalDevice.cpp
/// \ author: Thomas Clermont

#include "PhysicalDevice.h"


namespace SNG
{

	PhysicalDevice::PhysicalDevice(Ref<Instance> instance)
	{
		uint32_t gpuCount = 0;
		auto err = vkEnumeratePhysicalDevices(instance->Raw(), &gpuCount, nullptr);
		check_vk_result(err, "No GPU found");
		SNG_ASSERT(gpuCount > 0);

		VkPhysicalDevice* gpus = new VkPhysicalDevice[gpuCount];
		err = vkEnumeratePhysicalDevices(instance->Raw(), &gpuCount, gpus);
		check_vk_result(err, "No GPU found");

		int gpu = 0;
		for (int i = 0; i < (int)gpuCount; i++)
		{
			VkPhysicalDeviceProperties props;
			vkGetPhysicalDeviceProperties(gpus[i], &props);
			if (props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
			{
				gpu = i;
				break;
			}
		}

		m_PhysicalDevice = gpus[gpu];
		delete[] gpus;
	}

}