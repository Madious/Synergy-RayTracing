/// \ project: Synergy-RayTracing
/// \ file: Layer.h
/// \ author: Thomas Clermont

#pragma once
#include "Core.h"


namespace SNG
{

	class Layer
	{
	public:

		virtual void OnAttach() {}
		virtual void OnUpdate(float ts) {}
		virtual void OnRender() {}

	};

}