/// \ project: Synergy
/// \ file: Sync.h
/// \ author: Thomas Clermont

#pragma once
#include "Device.h"


namespace SNG
{

	class Sync
	{
	public:

		Sync(Ref<Device> device);
		~Sync();

		inline std::vector<VkSemaphore>& ImageAvaible() { return m_ImageAvaible; }
		inline std::vector<VkSemaphore>& RenderFinished() { return m_RenderFinished; }
		inline std::vector<VkFence>& InFlight() { return m_InFlight; }
		inline std::vector<VkFence>& ImageInFlight() { return m_ImageInFlight; }

	private:

		Ref<Device> r_Device;
		std::vector<VkSemaphore> m_ImageAvaible;
		std::vector<VkSemaphore> m_RenderFinished;
		std::vector<VkFence> m_InFlight;
		std::vector<VkFence> m_ImageInFlight;
	};

}
