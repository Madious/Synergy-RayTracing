/// \ project: RayTracing
/// \ file: AppLayer.cpp
/// \ author: Thomas Clermont

#include "AppLayer.h"
#include "SceneSerializer.h"

#include <Core/Application.h>
#include <Utils/FileDialogue.h>

#include <glm/gtc/type_ptr.hpp>
#include <imgui_stdlib.h>
#include <sstream>



AppLayer::AppLayer()
	: m_Camera(45.0f, 0.1f, 100.0f)
{
	{
		Sphere& sphere = m_Scene.Spheres.emplace_back();
		sphere.Name = "White";
		sphere.Position = { 0.0f, 0.0f, 0.0f };
		sphere.Radius = 1.0f;
		sphere.MaterialIndex = 0;

		Material& mat = m_Scene.Materials.emplace_back();
		mat.Name = "White Mat";
		mat.Albedo = { 1.0f, 1.0f, 1.0f };
		mat.EmissionColor = { 1.0f, 1.0f, 1.0f };
		mat.EmissionPower = 1.0f;
	}

	SNG::Application::GetApplication()->SetMenuBarCallback(std::bind(&AppLayer::DrawMenuBar, this));
}

void AppLayer::OnUpdate(float ts)
{
	if (m_Camera.OnUpdate(ts))
		m_RayTracer.ResetFrameIndex();
}

void AppLayer::OnRender()
{
	DrawSettings();
	DrawSceneHierarchy();
	DrawProperties();
	DrawMaterialEditor();
	DrawViewport();
}

void AppLayer::DrawMenuBar()
{
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("Save As..."))
		{
			auto filepath = SNG::FileDialogue::SaveFile("Synergy Scene (*.ssc)\0*.ssc\0");
			if (!filepath.empty())
			{
				SceneSerializer serializer(&m_Scene);
				serializer.Serialize(filepath);
			}
		}

		if (ImGui::MenuItem("Open..."))
		{
			auto filepath = SNG::FileDialogue::OpenFile("Synergy Scene (*.ssc)\0*.ssc\0");
			if (!filepath.empty())
			{
				Scene scene;
				SceneSerializer serializer(&scene);
				serializer.Deserialize(filepath);
				m_Scene = scene;
				m_RayTracer.ResetFrameIndex();
			}
		}

		ImGui::EndMenu();
	}
}

void AppLayer::DrawSettings()
{
	ImGui::Begin("Settings");

	ImGui::Text("FPS: %.0f", ImGui::GetIO().Framerate);
	ImGui::Text("Frame time: %.5fms", ImGui::GetIO().DeltaTime * 1000.0f);
	ImGui::Checkbox("Accumulate", &m_RayTracer.GetSettings().Accumulate);
	ImGui::SameLine();
	ImGui::Text("FrameIndex: %i", m_RayTracer.GetFrameIndex());
	if (ImGui::SliderInt("Bounces", &m_RayTracer.GetSettings().Bounces, 0, 100))
		m_RayTracer.ResetFrameIndex();

	ImGui::End();
}

void AppLayer::DrawSceneHierarchy()
{
	ImGui::Begin("Scene Hierarchy");
	if (ImGui::Button("Add+"))
	{
		std::stringstream ss;
		ss << "Sphere_" << m_Scene.Spheres.size();
		m_Scene.Spheres.push_back(Sphere{ ss.str(), glm::vec3(0.0f), 1.0f, 0});
		m_RayTracer.ResetFrameIndex();
	}

	for (size_t i = 0; i < m_Scene.Spheres.size(); i++)
	{
		auto& name = m_Scene.Spheres[i].Name;
		if (ImGui::Selectable(name != "" ? name.c_str() : "_"))
			m_SelectedObject = i;
	}

	ImGui::End();
}

void AppLayer::DrawProperties()
{
	ImGui::Begin("Properties");
	if (m_SelectedObject == -1)
	{
		ImGui::Text("Select an object to modify it.");
		ImGui::End();
		return;
	}

	Sphere& sphere = m_Scene.Spheres[m_SelectedObject];
	ImGui::InputText("Name", &sphere.Name);
	bool change =
		ImGui::DragFloat3("Position", glm::value_ptr(sphere.Position), 0.1f) ||
		ImGui::DragFloat("Radius", &sphere.Radius, 0.1f, 0.01f);

	if (ImGui::TreeNode("Material"))
	{
		for (size_t i = 0; i < m_Scene.Materials.size(); i++)
		{
			Material& mat = m_Scene.Materials[i];
			if (ImGui::Selectable(mat.Name != "" ? mat.Name.c_str() : "_"))
			{
				sphere.MaterialIndex = i;
				change = true;
			}
		}
		ImGui::TreePop();
	}

	if (change) m_RayTracer.ResetFrameIndex();
	ImGui::End();
}

void AppLayer::DrawMaterialEditor()
{
	ImGui::Begin("Material Editor");
	if (ImGui::Button("Add+"))
	{
		std::stringstream ss;
		ss << "Material_" << m_Scene.Materials.size();
		m_Scene.Materials.push_back(Material{ ss.str(), glm::vec3(1.0f), glm::vec3(1.0f), 0.0f });
		m_RayTracer.ResetFrameIndex();
	}

	for (size_t i = 0; i < m_Scene.Materials.size(); i++)
	{
		ImGui::PushID(i);
		Material& mat = m_Scene.Materials[i];

		ImGui::Text(mat.Name != "" ? mat.Name.c_str() : "_");
		ImGui::InputText("Name", &mat.Name);

		bool change =
			ImGui::ColorEdit3("Color", glm::value_ptr(mat.Albedo)) ||
			ImGui::ColorEdit3("Emission Color", glm::value_ptr(mat.EmissionColor)) ||
			ImGui::DragFloat("Emission Power", &mat.EmissionPower, 0.1f, 0.0f, 10.0f);

		ImGui::Separator();
		ImGui::PopID();
		if (change) m_RayTracer.ResetFrameIndex();
	}
	ImGui::End();
}

void AppLayer::DrawViewport()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("Viewport");

	m_ViewportWidth = ImGui::GetContentRegionAvail().x;
	m_ViewportHeight = ImGui::GetContentRegionAvail().y;

	m_Camera.OnResize(m_ViewportWidth, m_ViewportHeight);
	m_RayTracer.OnResize(m_ViewportWidth, m_ViewportHeight);
	m_RayTracer.Render(m_Camera, m_Scene);

	ImGui::Image(m_RayTracer.GetImage()->GetDescriptorSet(),
		{ (float)m_RayTracer.GetImage()->GetWidth(), (float)m_RayTracer.GetImage()->GetHeight() },
		ImVec2(0, 1), ImVec2(1, 0));

	ImGui::End();
	ImGui::PopStyleVar();
}
