/// \ project: Synergy
/// \ file: Messenger.h
/// \ author: Thomas Clermont

#pragma once
#include "Instance.h"


namespace SNG
{

	class Messenger
	{
	public:

		Messenger(Ref<Instance> instance);
		~Messenger();

		inline VkDebugUtilsMessengerEXT Raw() const { return m_Messenger; }

	private:

		Ref<Instance> r_Instance = nullptr;
		VkDebugUtilsMessengerEXT m_Messenger = VK_NULL_HANDLE;
	};

}