/// \ project: Synergy
/// \ file: Device.h
/// \ author: Thomas Clermont

#pragma once
#include "PhysicalDevice.h"


namespace SNG
{

	class Device
	{
	public:

		Device(Ref<PhysicalDevice> physicalDevice);
		~Device();

		inline VkDevice Raw() const { return m_Device; }
		inline uint32_t GetQueueFamily() const { return m_QueueFamily; }
		inline VkQueue GetQueue() const { return m_Queue; }

	private:

		uint32_t m_QueueFamily = (uint32_t)-1;
		VkDevice m_Device = VK_NULL_HANDLE;
		VkQueue m_Queue = VK_NULL_HANDLE;
	};

}