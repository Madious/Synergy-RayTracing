/// \ project: Synergy
/// \ file: CommandPool.cpp
/// \ author: Thomas Clermont

#include "CommandPool.h"


namespace SNG
{

	CommandPool::CommandPool(Ref<Device> device)
		: r_Device(device)
	{
		VkCommandPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		poolInfo.queueFamilyIndex = r_Device->GetQueueFamily();

		auto err = vkCreateCommandPool(r_Device->Raw(), &poolInfo, nullptr, &m_CommandPool);
		check_vk_result(err, "Failed to created a command pool");
	}

	CommandPool::~CommandPool()
	{
		vkDestroyCommandPool(r_Device->Raw(), m_CommandPool, nullptr);
	}

}
