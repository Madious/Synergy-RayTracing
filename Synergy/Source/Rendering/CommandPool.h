/// \ project: Synergy
/// \ file: CommandPool.h
/// \ author: Thomas Clermont

#pragma once
#include "Device.h"


namespace SNG
{

	class CommandPool
	{
	public:

		CommandPool(Ref<Device> device);
		~CommandPool();

		inline VkCommandPool Raw() const { return m_CommandPool; }

	private:

		Ref<Device> r_Device;
		VkCommandPool m_CommandPool = VK_NULL_HANDLE;
	};

}
