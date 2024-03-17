/// \ project: Synergy
/// \ file: Input.cpp
/// \ author: Thomas Clermont

#include "Input.h"
#include "Core/Application.h"


namespace SNG
{

	bool Input::IsKeyPress(int key)
	{
		auto window = Application::GetApplication()->GetWindow()->Raw();
		int action = glfwGetKey(window, key);
		return action == GLFW_PRESS;
	}

	bool Input::IsKeyRelease(int key)
	{
		auto window = Application::GetApplication()->GetWindow()->Raw();
		int action = glfwGetKey(window, key);
		return action == GLFW_RELEASE;
	}

	bool Input::IsMouseButtonPress(int key)
	{
		auto window = Application::GetApplication()->GetWindow()->Raw();
		int action = glfwGetMouseButton(window, key);
		return action == GLFW_PRESS;
	}

	bool Input::IsMouseButtonRelease(int key)
	{
		auto window = Application::GetApplication()->GetWindow()->Raw();
		int action = glfwGetMouseButton(window, key);
		return action == GLFW_RELEASE;
	}

	glm::vec2 Input::GetMousePos()
	{
		auto window = Application::GetApplication()->GetWindow()->Raw();
		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);
		return glm::vec2((float)xPos, (float)yPos);
	}

	void Input::SetCursorMode(int mode)
	{
		auto window = Application::GetApplication()->GetWindow()->Raw();
		glfwSetInputMode(window, GLFW_CURSOR, mode);
	}

}