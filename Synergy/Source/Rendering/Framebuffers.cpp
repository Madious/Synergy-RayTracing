/// \ project: Synergy
/// \ file: Framebuffers.cpp
/// \ author: Thomas Clermont

#include "Framebuffers.h"


namespace SNG
{

	Framebuffers::Framebuffers(Ref<Device> device, Ref<Swapchain> swapchain, Ref<RenderPass> renderPass)
		: r_Device(device)
	{
		m_Framebuffers.resize(MAX_IMAGE_COUNT);
		for (uint32_t i = 0; i < MAX_IMAGE_COUNT; i++)
		{
			VkFramebufferCreateInfo framebufferInfo{};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.pAttachments = &swapchain->GetImageViews()[i];
			framebufferInfo.attachmentCount = 1;
			framebufferInfo.renderPass = renderPass->Raw();
			framebufferInfo.width = swapchain->GetImageExtent().width;
			framebufferInfo.height = swapchain->GetImageExtent().height;
			framebufferInfo.layers = 1;

			auto err = vkCreateFramebuffer(r_Device->Raw(), &framebufferInfo, nullptr, &m_Framebuffers[i]);
			check_vk_result(err, "Failed to create a framebuffer");
		}
	}

	Framebuffers::~Framebuffers()
	{
		for (auto framebuffer : m_Framebuffers)
			vkDestroyFramebuffer(r_Device->Raw(), framebuffer, nullptr);
	}

}
