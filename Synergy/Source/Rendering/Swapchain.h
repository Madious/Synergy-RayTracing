/// \ project: Synergy
/// \ file: Swapchain.h
/// \ author: Thomas Clermont

#pragma once
#include "Surface.h"
#include "Device.h"


namespace SNG
{

	class Swapchain
	{
	public:

		Swapchain(Window* window, Ref<PhysicalDevice> physicalDevice, Ref<Device> device, Ref<Surface> surface);
		~Swapchain();

		inline VkSwapchainKHR Raw() const { return m_Swapchain; }
		inline VkExtent2D GetImageExtent() const { return m_ImageExtent; }
		inline VkFormat GetImageFormat() const { return m_ImageFormat; }
		inline uint32_t GetImageCount() const { return m_ImageCount; }
		inline const std::vector<VkImage>& GetImages() const { return m_Images; }
		inline const std::vector<VkImageView>& GetImageViews() const { return m_ImageViews; }

	private:

		Ref<Device> r_Device;

		VkSwapchainKHR m_Swapchain = VK_NULL_HANDLE;
		VkExtent2D m_ImageExtent;
		VkFormat m_ImageFormat;
		uint32_t m_ImageCount;
		std::vector<VkImage> m_Images;
		std::vector<VkImageView> m_ImageViews;
	};

}
