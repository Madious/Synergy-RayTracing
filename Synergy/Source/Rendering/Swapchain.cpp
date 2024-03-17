/// \ project: Synergy
/// \ file: Swapchain.cpp
/// \ author: Thomas Clermont

#include "Swapchain.h"


namespace SNG
{

	Swapchain::Swapchain(Window* window, Ref<PhysicalDevice> physicalDevice,
		Ref<Device> device, Ref<Surface> surface)
		: r_Device(device)
	{
		//----- Swapchain
		VkExtent2D extent = { 0, 0 };
		VkSurfaceCapabilitiesKHR capabilities;
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice->Raw(), surface->Raw(), &capabilities);

		if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
		{
			extent = capabilities.currentExtent;
		}
		else
		{
			int windowWidth = window->GetWidth();
			int windowHeight = window->GetHeight();
			extent = { (uint32_t)windowWidth, (uint32_t)windowHeight };
			extent.width = std::clamp(extent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
			extent.height = std::clamp(extent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
		}

		VkFormat format = VK_FORMAT_B8G8R8A8_UNORM;
		VkPresentModeKHR presentMode = VK_PRESENT_MODE_MAILBOX_KHR;

		uint32_t imageCount = capabilities.minImageCount + 1;
		if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount)
			imageCount = capabilities.minImageCount;

		auto queueFamily = r_Device->GetQueueFamily();

		VkSwapchainCreateInfoKHR swapchainInfo{};
		swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapchainInfo.imageColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
		swapchainInfo.imageFormat = format;
		swapchainInfo.imageExtent = extent;
		swapchainInfo.presentMode = presentMode;
		swapchainInfo.minImageCount = imageCount;
		swapchainInfo.surface = surface->Raw();
		swapchainInfo.clipped = VK_TRUE;
		swapchainInfo.pQueueFamilyIndices = &queueFamily;
		swapchainInfo.preTransform = capabilities.currentTransform;
		swapchainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		swapchainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		swapchainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swapchainInfo.oldSwapchain = VK_NULL_HANDLE;
		swapchainInfo.queueFamilyIndexCount = 1;
		swapchainInfo.imageArrayLayers = 1;

		auto err = vkCreateSwapchainKHR(r_Device->Raw(), &swapchainInfo, nullptr, &m_Swapchain);
		check_vk_result(err, "Failed to created a swapchain");

		//----- Images
		m_ImageExtent = extent;
		m_ImageFormat = format;
		vkGetSwapchainImagesKHR(r_Device->Raw(), m_Swapchain, &m_ImageCount, nullptr);
		m_Images.resize(m_ImageCount);
		vkGetSwapchainImagesKHR(r_Device->Raw(), m_Swapchain, &m_ImageCount, m_Images.data());
		m_ImageViews.resize(m_ImageCount);

		//----- Image Views
		for (uint32_t i = 0; i < m_ImageCount; i++)
		{
			VkImageViewCreateInfo viewInfo{};
			viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			viewInfo.image = m_Images[i];
			viewInfo.format = format;
			viewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			viewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			viewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			viewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			viewInfo.subresourceRange.baseMipLevel = 0;
			viewInfo.subresourceRange.levelCount = 1;
			viewInfo.subresourceRange.baseArrayLayer = 0;
			viewInfo.subresourceRange.layerCount = 1;

			err = vkCreateImageView(r_Device->Raw(), &viewInfo, nullptr, &m_ImageViews[i]);
			check_vk_result(err, "Failed to created an image view");
		}
	}

	Swapchain::~Swapchain()
	{
		for (auto imageView : m_ImageViews)
			vkDestroyImageView(r_Device->Raw(), imageView, nullptr);
		
		vkDestroySwapchainKHR(r_Device->Raw(), m_Swapchain, nullptr);
	}

}
