/// \ project: Synergy
/// \ file: Input.h
/// \ author: Thomas Clermont

#pragma once
#include "Core/Core.h"


namespace SNG
{
	class Input
	{
	public:

		static bool IsKeyPress(int key);
		static bool IsKeyRelease(int key);
		static bool IsMouseButtonPress(int key);
		static bool IsMouseButtonRelease(int key);
		static glm::vec2 GetMousePos();
		static void SetCursorMode(int mode);

	};
}