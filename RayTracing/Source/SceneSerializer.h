/// \ project: RayTracing
/// \ file: SceneSerializer.h
/// \ author: Thomas Clermont

#pragma once
#include "Scene.h"


class SceneSerializer
{
public:

	SceneSerializer(Scene* scene);

	void Serialize(const std::string& filepath);
	void Deserialize(const std::string& filepath);

private:

	Scene* m_Scene;
};
