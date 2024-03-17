/// \ project: RayTracing
/// \ file: Scene.h
/// \ author: Thomas Clermont

#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <string>



struct Sphere
{
	std::string Name;
	glm::vec3 Position;
	float Radius;
	int MaterialIndex;
};

struct Material
{
	std::string Name;
	glm::vec3 Albedo;
	glm::vec3 EmissionColor;
	float EmissionPower;

	glm::vec3 GetEmission() const { return EmissionColor * EmissionPower; }
};

struct Scene
{
	std::vector<Sphere> Spheres;
	std::vector<Material> Materials;
};
