/// \ project: Synergy
/// \ file: PhysicalDevice.h
/// \ author: Thomas Clermont

#pragma once
#include "Instance.h"


namespace SNG
{

	class PhysicalDevice
	{
	public:

		PhysicalDevice(Ref<Instance> instance);
		inline VkPhysicalDevice Raw() const { return m_PhysicalDevice; }

	private:

		VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
	};

}