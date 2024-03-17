/// \ project: RayTracing
/// \ file: SceneSerializer.cpp
/// \ author: Thomas Clermont

#include "SceneSerializer.h"
#include <yaml-cpp/yaml.h>
#include <fstream>



//===== Operators
namespace YAML {

	Emitter& operator<<(Emitter& out, const glm::vec3& vec)
	{
		out << Flow;
		out << BeginSeq << vec.x << vec.y << vec.z << EndSeq;
		return out;
	}

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};
}





SceneSerializer::SceneSerializer(Scene* scene)
	: m_Scene(scene)
{
}

void SceneSerializer::Serialize(const std::string& filepath)
{
	YAML::Emitter out;
	out << YAML::BeginMap;


	//===== Objects
	out << YAML::Key << "Objects" << YAML::Value << YAML::BeginSeq;
	for (const auto& sphere : m_Scene->Spheres)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Name" << YAML::Value << sphere.Name;
		out << YAML::Key << "Position" << YAML::Value << sphere.Position;
		out << YAML::Key << "Radius" << YAML::Value << sphere.Radius;
		out << YAML::Key << "MaterialIndex" << YAML::Value << sphere.MaterialIndex;
		out << YAML::EndMap;
	}
	out << YAML::EndSeq;


	//===== Materials
	out << YAML::Key << "Materials" << YAML::Value << YAML::BeginSeq;
	for (const auto& mat : m_Scene->Materials)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Name" << YAML::Value << mat.Name;
		out << YAML::Key << "Albedo" << YAML::Value << mat.Albedo;
		out << YAML::Key << "EmissionColor" << YAML::Value << mat.EmissionColor;
		out << YAML::Key << "EmissionPower" << YAML::Value << mat.EmissionPower;
		out << YAML::EndMap;
	}
	out << YAML::EndSeq;


	//===== Write
	out << YAML::EndMap;
	std::ofstream fout(filepath);
	fout << out.c_str();
}

void SceneSerializer::Deserialize(const std::string& filepath)
{
	//===== Load
	std::ifstream fin(filepath);
	std::stringstream strStream;
	strStream << fin.rdbuf();
	YAML::Node data = YAML::Load(strStream.str());

	//===== Objects
	auto objects = data["Objects"];
	if (objects)
	{
		for (auto object : objects)
		{
			Sphere& sphere = m_Scene->Spheres.emplace_back();
			sphere.Name = object["Name"].as<std::string>();
			sphere.Position = object["Position"].as<glm::vec3>();
			sphere.Radius = object["Radius"].as<float>();
			sphere.MaterialIndex = object["MaterialIndex"].as<int>();
		}
	}

	//===== Materials
	auto materials = data["Materials"];
	if (materials)
	{
		for (auto material : materials)
		{
			Material& mat = m_Scene->Materials.emplace_back();
			mat.Name = material["Name"].as<std::string>();
			mat.Albedo = material["Albedo"].as<glm::vec3>();
			mat.EmissionColor = material["EmissionColor"].as<glm::vec3>();
			mat.EmissionPower = material["EmissionPower"].as<float>();
		}
	}
}
