/// \ project: Synergy
/// \ file: Image.cpp
/// \ author: Thomas Clermont

#include "Image.h"
#include "Core/Application.h"


namespace SNG
{
	static uint32_t FindMemoryType(VkMemoryPropertyFlags properties, uint32_t type)
	{
		VkPhysicalDeviceMemoryProperties memProps;
		vkGetPhysicalDeviceMemoryProperties(Application::GetPhysicalDevice(), &memProps);

		for (uint32_t i = 0; i < memProps.memoryTypeCount; i++)
		{
			if (type & (1 << i) && (memProps.memoryTypes[i].propertyFlags & properties) == properties)
				return i;
		}

		return 0xffffffff;
	}



	Image::Image(uint32_t width, uint32_t height)
		: m_Width(width), m_Height(height)
	{
		AllocateMemory();
	}

	Image::~Image()
	{
		Release();
	}

	void Image::SetData(void* data)
	{
		size_t uploadSize = m_Width * m_Height * 4;

		if (!m_StaginBuffer)
		{
			// Create the stagin buffer
			{
				VkBufferCreateInfo bufferInfo{};
				bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
				bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
				bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
				bufferInfo.size = uploadSize;
				auto err = vkCreateBuffer(Application::GetDevice(), &bufferInfo, nullptr, &m_StaginBuffer);
				check_vk_result(err, "Erreur lors de la creation d'un buffer");

				VkMemoryRequirements requirements;
				vkGetBufferMemoryRequirements(Application::GetDevice(), m_StaginBuffer, &requirements);

				VkMemoryAllocateInfo allocInfo{};
				allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
				allocInfo.allocationSize = requirements.size;
				allocInfo.memoryTypeIndex = FindMemoryType( VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
					VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, requirements.memoryTypeBits);
				err = vkAllocateMemory(Application::GetDevice(), &allocInfo, nullptr, &m_StaginBufferMemory);
				check_vk_result(err, "Echec de l'allocation de memoire");

				err = vkBindBufferMemory(Application::GetDevice(), m_StaginBuffer, m_StaginBufferMemory, 0);
				check_vk_result(err, "Echec de l'assaciation de la memoire");
			}
		}

		// Upload to buffer
		{
			void* memData;
			vkMapMemory(Application::GetDevice(), m_StaginBufferMemory, 0, uploadSize, 0, &memData);
			memcpy(memData, data, uploadSize);
			VkMappedMemoryRange range{};
			range.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
			range.memory = m_StaginBufferMemory;
			range.size = uploadSize;
			vkFlushMappedMemoryRanges(Application::GetDevice(), 1, &range);
			vkUnmapMemory(Application::GetDevice(), m_StaginBufferMemory);
		}

		// Record command buffer
		{
			auto commandBuffer = Application::GetApplication()->CreateCommandBuffer();

			VkImageMemoryBarrier barrier{};
			barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrier.image = m_Image;
			barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			barrier.subresourceRange.levelCount = 1;
			barrier.subresourceRange.layerCount = 1;
			vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
				VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);

			VkBufferImageCopy region{};
			region.imageExtent = { m_Width, m_Height, 1 };
			region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			region.imageSubresource.layerCount = 1;
			vkCmdCopyBufferToImage(commandBuffer, m_StaginBuffer,
				m_Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;;
			barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT,
				VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);
			vkEndCommandBuffer(commandBuffer);

			Application::GetApplication()->FlushCommandBuffers(commandBuffer);
		}
	}

	void Image::Resize(uint32_t width, uint32_t height)
	{
		if (width == m_Width && height == m_Height)
			return;

		m_Width = width;
		m_Height = height;

		Release();
		AllocateMemory();
	}

	void Image::AllocateMemory()
	{
		// Create Image
		{
			VkImageCreateInfo imageInfo{};
			imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
			imageInfo.imageType = VK_IMAGE_TYPE_2D;
			imageInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
			imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
			imageInfo.extent = { m_Width, m_Height, 1 };
			imageInfo.arrayLayers = 1;
			imageInfo.mipLevels = 1;
			imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
			imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
			imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			imageInfo.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
			auto err = vkCreateImage(Application::GetDevice(), &imageInfo, nullptr, &m_Image);
			check_vk_result(err, "Echec de la creation d'une image");

			VkMemoryRequirements requirements;
			vkGetImageMemoryRequirements(Application::GetDevice(), m_Image, &requirements);

			VkMemoryAllocateInfo allocInfo{};
			allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			allocInfo.allocationSize = requirements.size;
			allocInfo.memoryTypeIndex = FindMemoryType(
				VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, requirements.memoryTypeBits);
			err = vkAllocateMemory(Application::GetDevice(), &allocInfo, nullptr, &m_Memory);
			check_vk_result(err, "Echec de l'allocation de memoire");

			err = vkBindImageMemory(Application::GetDevice(), m_Image, m_Memory, 0);
			check_vk_result(err, "Echec de l'assaciation de la memoire");
		}

		// Create image view
		{
			VkImageViewCreateInfo viewInfo{};
			viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			viewInfo.image = m_Image;
			viewInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
			viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			viewInfo.subresourceRange.baseMipLevel = 0;
			viewInfo.subresourceRange.levelCount = 1;
			viewInfo.subresourceRange.baseArrayLayer = 0;
			viewInfo.subresourceRange.layerCount = 1;
			auto err = vkCreateImageView(Application::GetDevice(), &viewInfo, nullptr, &m_ImageView);
			check_vk_result(err, "Erreur lors de la creation d'une image view");
		}

		// Create sampler
		{
			VkSamplerCreateInfo samplerInfo{};
			samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
			samplerInfo.minFilter = VK_FILTER_LINEAR;
			samplerInfo.magFilter = VK_FILTER_LINEAR;
			samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
			samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			samplerInfo.minLod = -1000;
			samplerInfo.maxLod = 1000;
			samplerInfo.maxAnisotropy = 1.0f;
			auto err = vkCreateSampler(Application::GetDevice(), &samplerInfo, nullptr, &m_Sampler);
			check_vk_result(err, "Echec de la creation d'un sampler");
		}

		// Descriptor set
		m_DescriptorSet = ImGui_ImplVulkan_AddTexture(m_Sampler, m_ImageView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
	}

	void Image::Release()
	{
		Application::GetApplication()->AddClearFunction([sampler = m_Sampler, imageView = m_ImageView,
			image = m_Image, memory = m_Memory, buffer = m_StaginBuffer, bufferMemory = m_StaginBufferMemory](){
			vkDestroySampler(Application::GetDevice(), sampler, nullptr);
			vkDestroyImageView(Application::GetDevice(), imageView, nullptr);
			vkDestroyImage(Application::GetDevice(), image, nullptr);
			vkFreeMemory(Application::GetDevice(), memory, nullptr);
			vkDestroyBuffer(Application::GetDevice(), buffer, nullptr);
			vkFreeMemory(Application::GetDevice(), bufferMemory, nullptr);
		});

		m_Sampler = nullptr;
		m_ImageView = nullptr;
		m_Image = nullptr;
		m_Memory = nullptr;
		m_StaginBuffer = nullptr;
		m_StaginBufferMemory = nullptr;
	}

}
