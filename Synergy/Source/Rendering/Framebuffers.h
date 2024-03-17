/// \ project: Synergy
/// \ file: Framebuffers.h
/// \ author: Thomas Clermont

#pragma once
#include "Swapchain.h"
#include "RenderPass.h"


namespace SNG
{

	class Framebuffers
	{
	public:

		Framebuffers(Ref<Device> device, Ref<Swapchain> swapchain, Ref<RenderPass> renderPass);
		~Framebuffers();

		inline const std::vector<VkFramebuffer>& Raw() const { return m_Framebuffers; }

	private:

		Ref<Device> r_Device;
		std::vector<VkFramebuffer> m_Framebuffers;
	};

}
