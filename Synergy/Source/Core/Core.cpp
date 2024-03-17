/// \ project: Synergy-RayTracing
/// \ file: Core.cpp
/// \ author: Thomas Clermont

#include "Core.h"
#include "Application.h"


namespace SNG
{
	void check_vk_result(VkResult result, const char* message)
	{
		if (result != VK_SUCCESS)
		{
			std::cout << message << "\n";
			__debugbreak();
		}
	}

	VKAPI_ATTR VkBool32 VKAPI_CALL VulkanErrorCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallback,
		void* pUserData)
	{
		std::cout << "[VULKAN ERROR]: " << pCallback->pMessage << "\n";
		return VK_FALSE;
	}

	

	VkCommandBuffer OneTimeCommandBuffer()
	{
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = Application::GetCommandPool();
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		auto err = vkAllocateCommandBuffers(Application::GetDevice(), &allocInfo, &commandBuffer);
		check_vk_result(err, "Echec de l'allocation d'un command buffer");

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		err = vkBeginCommandBuffer(commandBuffer, &beginInfo);
		check_vk_result(err, "Echec de l'enregistrement d'un command buffer");

		return commandBuffer;
	}

	void EndOneTimeCommandBuffer(VkCommandBuffer commandBuffer)
	{
		VkSubmitInfo submit{};
		submit.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submit.commandBufferCount = 1;
		submit.pCommandBuffers = &commandBuffer;

		vkEndCommandBuffer(commandBuffer);
		auto err = vkQueueSubmit(Application::GetQueue(), 1, &submit, VK_NULL_HANDLE);
		check_vk_result(err, "Erreur de la soumission d'un command buffer");
		vkQueueWaitIdle(Application::GetQueue());
	}

}
