/// \ project: Synergy
/// \ file: Random.cpp
/// \ author: Thomas Clermont

#include "Random.h"


namespace SNG
{
	thread_local std::mt19937 Random::s_RandomEngine;
	std::uniform_int_distribution<std::mt19937::result_type> Random::s_Distribution;


	// Random
	void Random::Initialize()
	{
		s_RandomEngine.seed(std::random_device()());
	}

	uint32_t Random::RandomUINT()
	{
		return s_Distribution(s_RandomEngine);
	}

	uint32_t Random::RandomUINT(uint32_t min, uint32_t max)
	{
		return min + (s_Distribution(s_RandomEngine) % (max - min + 1));
	}

	float Random::RandomFloat()
	{
		return (float)RandomUINT() / (float)std::numeric_limits<uint32_t>::max();
	}

	glm::vec3 Random::RandomVec3()
	{
		return glm::vec3(RandomFloat(), RandomFloat(), RandomFloat());
	}

	glm::vec3 Random::RandomVec3(float min, float max)
	{
		return glm::vec3(
			RandomFloat() * (max - min) + min,
			RandomFloat() * (max - min) + min,
			RandomFloat() * (max - min) + min
		);
	}

	glm::vec3 Random::UnitSphere()
	{
		return glm::normalize(RandomVec3(-1.0f, 1.0f));
	}




	// Fast
	uint32_t Random::FastUINT(uint32_t& input)
	{
		uint32_t state = input * 747796405u + 2891336453u;
		uint32_t word = ((state >> ((state >> 28u) + 4u)) ^ state) * 277803737u;
		return (word >> 22u) ^ word;
	}

	uint32_t Random::FastUINT(uint32_t& input, uint32_t min, uint32_t max)
	{
		return min + (FastUINT(input) % (max - min + 1));
	}

	float Random::FastFloat(uint32_t& input)
	{
		input = FastUINT(input);
		return (float)input / (float)std::numeric_limits<uint32_t>::max();
	}

	glm::vec3 Random::FastVec3(uint32_t& input)
	{
		return glm::vec3(FastFloat(input), FastFloat(input), FastFloat(input));
	}

	glm::vec3 Random::FastVec3(uint32_t& input, float min, float max)
	{
		return glm::vec3(
			FastFloat(input) * (max - min) + min,
			FastFloat(input) * (max - min) + min,
			FastFloat(input) * (max - min) + min
		);
	}

	glm::vec3 Random::FastUnitSphere(uint32_t& input)
	{
		return glm::normalize(glm::vec3(FastVec3(input, -1.0f, 1.0f)));
	}

}