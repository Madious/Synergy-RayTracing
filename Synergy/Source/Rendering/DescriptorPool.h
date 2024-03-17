/// \ project: Synergy
/// \ file: DescriptorPool.h
/// \ author: Thomas Clermont

#pragma once
#include "Device.h"


namespace SNG
{

	class DescriptorPool
	{
	public:

		DescriptorPool(Ref<Device> device, size_t descriptorCount);
		~DescriptorPool();

		inline VkDescriptorPool Raw() const { return m_DescriptorPool; }

	private:

		Ref<Device> r_Device = nullptr;
		VkDescriptorPool m_DescriptorPool = VK_NULL_HANDLE;
	};

}
