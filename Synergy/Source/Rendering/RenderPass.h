/// \ project: Synergy
/// \ file: RenderPass.h
/// \ author: Thomas Clermont

#pragma once
#include "Device.h"


namespace SNG
{

	class RenderPass
	{
	public:

		RenderPass(Ref<Device> device);
		~RenderPass();

		inline VkRenderPass Raw() const { return m_RenderPass; }

	private:

		Ref<Device> r_Device;
		VkRenderPass m_RenderPass = VK_NULL_HANDLE;
	};

}
