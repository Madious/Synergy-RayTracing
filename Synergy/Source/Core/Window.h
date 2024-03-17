/// \ project: Synergy
/// \ file: Window.h
/// \ author: Thomas Clermont

#pragma once
#include "Core.h"


namespace SNG
{

	class Window
	{
	public:

		Window(int width, int height, const std::string& title);
		~Window();

		int GetWidth() const;
		int GetHeight() const;
		bool IsClose() const;
		void OnUpdate();

		void SetFramebufferSizeCallback(void(*func)(GLFWwindow*, int, int));
		inline GLFWwindow* Raw() const { return m_Window; }

	private:

		GLFWwindow* m_Window;
	};

}