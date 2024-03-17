/// \ project: Synergy
/// \ file: Sync.cpp
/// \ author: Thomas Clermont

#include "Sync.h"


namespace SNG
{

	Sync::Sync(Ref<Device> device)
		: r_Device(device)
	{
		VkSemaphoreCreateInfo semaphoreInfo{};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		VkFenceCreateInfo fenceInfo{};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		m_ImageAvaible.resize(MAX_IMAGE_COUNT);
		m_RenderFinished.resize(MAX_IMAGE_COUNT);
		m_InFlight.resize(MAX_IMAGE_COUNT);
		m_ImageInFlight.resize(MAX_IMAGE_COUNT, VK_NULL_HANDLE);

		for (uint32_t i = 0; i < MAX_IMAGE_COUNT; i++)
		{
			vkCreateSemaphore(r_Device->Raw(), &semaphoreInfo, nullptr, &m_ImageAvaible[i]);
			vkCreateSemaphore(r_Device->Raw(), &semaphoreInfo, nullptr, &m_RenderFinished[i]);
			vkCreateFence(r_Device->Raw(), &fenceInfo, nullptr, &m_InFlight[i]);
		}
	}

	Sync::~Sync()
	{
		for (uint32_t i = 0; i < MAX_IMAGE_COUNT; i++)
		{
			vkDestroySemaphore(r_Device->Raw(), m_ImageAvaible[i], nullptr);
			vkDestroySemaphore(r_Device->Raw(), m_RenderFinished[i], nullptr);
			vkDestroyFence(r_Device->Raw(), m_InFlight[i], nullptr);
		}
	}

}
