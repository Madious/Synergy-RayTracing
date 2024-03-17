/// \ project: Synergy
/// \ file: Instance.h
/// \ author: Thomas Clermont

#pragma once
#include "Core/Core.h"


namespace SNG
{

	class Instance
	{
	public:

		Instance();
		~Instance();
		inline VkInstance Raw() const { return m_Instance; }

	private:

		VkInstance m_Instance = VK_NULL_HANDLE;

	};

}