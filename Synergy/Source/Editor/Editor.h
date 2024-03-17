/// \ project: Synergy-RayTracing
/// \ file: Editor.h
/// \ author: Thomas Clermont

#pragma once
#include "Core/Core.h"


namespace SNG
{

	class Editor
	{
	public:

		void Init();
		~Editor();

		void SetMenuBarCallback(std::function<void()> func) { m_MenuBarCallback = func; }
		void RecordCommandBuffer(uint32_t index);
		void DrawFrame();

	private:

		std::function<void()> m_MenuBarCallback;

	};

}
