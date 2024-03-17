/// \ project: Synergy
/// \ file: Surface.h
/// \ author: Thomas Clermont

#pragma once
#include "Core/Window.h"
#include "Instance.h"


namespace SNG
{

	class Surface
	{
	public:

		Surface(Window* window, Ref<Instance> instance);
		~Surface();

		inline VkSurfaceKHR Raw() const { return m_Surface; }

	private:

		Ref<Instance> r_Instance;
		VkSurfaceKHR m_Surface = VK_NULL_HANDLE;
	};

}