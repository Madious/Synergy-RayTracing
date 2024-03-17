/// \ project: RayTracing
/// \ file: AppLayer.h
/// \ author: Thomas Clermont

#pragma once
#include <Core/Layer.h>
#include "Rendering/RayTracer.h"



class AppLayer : public SNG::Layer
{
public:

	AppLayer();

	void OnUpdate(float ts) override;
	void OnRender() override;

private:

	void DrawMenuBar();
	void DrawSettings();
	void DrawSceneHierarchy();
	void DrawProperties();
	void DrawMaterialEditor();
	void DrawViewport();

private:

	RayTracer m_RayTracer;
	Camera m_Camera;
	Scene m_Scene;
	uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
	int m_SelectedObject = -1;
};
