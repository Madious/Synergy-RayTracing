/// \ project: Synergy
/// \ file: Image.h
/// \ author: Thomas Clermont

#pragma once
#include "Core/Core.h"


namespace SNG
{

	class Image
	{
	public:

		Image(uint32_t width, uint32_t height);
		~Image();

		void SetData(void* data);
		void Resize(uint32_t width, uint32_t height);

		inline uint32_t GetWidth() const { return m_Width; }
		inline uint32_t GetHeight() const { return m_Height; }
		inline VkDescriptorSet GetDescriptorSet() const { return m_DescriptorSet; }

	private:

		void AllocateMemory();
		void Release();

	private:

		uint32_t m_Width, m_Height;

		VkImage m_Image = nullptr;
		VkDeviceMemory m_Memory = nullptr;
		VkImageView m_ImageView = nullptr;
		VkSampler m_Sampler = nullptr;
		VkDescriptorSet m_DescriptorSet = nullptr;

		VkBuffer m_StaginBuffer = nullptr;
		VkDeviceMemory m_StaginBufferMemory = nullptr;

	};

}