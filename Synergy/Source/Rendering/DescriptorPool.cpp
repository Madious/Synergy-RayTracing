/// \ project: Synergy
/// \ file: DescriptorPool.cpp
/// \ author: Thomas Clermont

#include "DescriptorPool.h"


namespace SNG
{

	DescriptorPool::DescriptorPool(Ref<Device> device, size_t descriptorCount)
		: r_Device(device)
	{
		VkDescriptorPoolSize poolSizes[] = {
			{ VK_DESCRIPTOR_TYPE_SAMPLER, descriptorCount },
			{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, descriptorCount },
			{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, descriptorCount },
			{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, descriptorCount },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, descriptorCount },
			{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, descriptorCount },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, descriptorCount },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, descriptorCount },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, descriptorCount },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, descriptorCount },
			{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, descriptorCount }
		};

		VkDescriptorPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		poolInfo.maxSets = descriptorCount * IM_ARRAYSIZE(poolSizes);
		poolInfo.poolSizeCount = (uint32_t)IM_ARRAYSIZE(poolSizes);
		poolInfo.pPoolSizes = poolSizes;

		auto err = vkCreateDescriptorPool(r_Device->Raw(), &poolInfo, nullptr, &m_DescriptorPool);
		check_vk_result(err, "Failed to created a descriptor pool");
	}

	DescriptorPool::~DescriptorPool()
	{
		vkDestroyDescriptorPool(r_Device->Raw(), m_DescriptorPool, nullptr);
	}

}
