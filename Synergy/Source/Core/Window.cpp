/// \ project: Synergy
/// \ file: Window.cpp
/// \ author: Thomas Clermont

#include "Window.h"


namespace SNG
{

	static void GLFWCallback(int code, const char* desc)
	{
		std::cout << "[GLFW ERROR]: (" << code << ") -> " << desc << "\n";
	}


	Window::Window(int width, int height, const std::string& title)
	{
		glfwInit();
		glfwSetErrorCallback(GLFWCallback);
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		m_Window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	}

	Window::~Window()
	{
		glfwDestroyWindow(m_Window);
		glfwTerminate();
	}

	int Window::GetWidth() const
	{
		int width;
		glfwGetFramebufferSize(m_Window, &width, NULL);
		return width;
	}

	int Window::GetHeight() const
	{
		int height;
		glfwGetFramebufferSize(m_Window, NULL, &height);
		return height;
	}

	bool Window::IsClose() const
	{
		return glfwWindowShouldClose(m_Window);
	}

	void Window::OnUpdate()
	{
		glfwPollEvents();
	}

	void Window::SetFramebufferSizeCallback(void(*func)(GLFWwindow*, int, int))
	{
		glfwSetFramebufferSizeCallback(m_Window, func);
	}

}