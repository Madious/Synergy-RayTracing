/// \ project: Synergy
/// \ file: RenderPass.cpp
/// \ author: Thomas Clermont

#include "RenderPass.h"


namespace SNG
{

	RenderPass::RenderPass(Ref<Device> device)
		: r_Device(device)
	{
		VkAttachmentDescription attachments[1] = {};
		attachments[0].format = VK_FORMAT_B8G8R8A8_UNORM;
		attachments[0].samples = VK_SAMPLE_COUNT_1_BIT;
		attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		attachments[0].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		VkAttachmentReference references[1] = {};
		references[0].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		references[0].attachment = 0;

		VkSubpassDescription subpass[1] = {};
		subpass[0].pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass[0].colorAttachmentCount = 1;
		subpass[0].pColorAttachments = references;

		VkSubpassDependency dependencies[1] = {};
		dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
		dependencies[0].dstSubpass = 0;
		dependencies[0].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependencies[0].srcAccessMask = 0;
		dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		VkRenderPassCreateInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = (uint32_t)IM_ARRAYSIZE(attachments);
		renderPassInfo.subpassCount = (uint32_t)IM_ARRAYSIZE(subpass);
		renderPassInfo.dependencyCount = (uint32_t)IM_ARRAYSIZE(dependencies);
		renderPassInfo.pAttachments = attachments;
		renderPassInfo.pSubpasses = subpass;
		renderPassInfo.pDependencies = dependencies;

		auto err = vkCreateRenderPass(r_Device->Raw(), &renderPassInfo, nullptr, &m_RenderPass);
		check_vk_result(err, "Failed to created a render pass");
	}

	RenderPass::~RenderPass()
	{
		vkDestroyRenderPass(r_Device->Raw(), m_RenderPass, nullptr);
	}

}
