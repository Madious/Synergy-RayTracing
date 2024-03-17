/// \ project: Synergy
/// \ file: Surface.cpp
/// \ author: Thomas Clermont

#include "Surface.h"


namespace SNG
{

	Surface::Surface(Window* window, Ref<Instance> instance)
		: r_Instance(instance)
	{
		auto err = glfwCreateWindowSurface(r_Instance->Raw(), window->Raw(), nullptr, &m_Surface);
		check_vk_result(err, "Failed to created a surface");
	}

	Surface::~Surface()
	{
		vkDestroySurfaceKHR(r_Instance->Raw(), m_Surface, nullptr);
	}

}
