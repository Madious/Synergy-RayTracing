/// \ project: Synergy
/// \ file: Random.h
/// \ author: Thomas Clermont

#pragma once
#include "Core/Core.h"


namespace SNG
{

	class Random
	{
	public:

		static void Initialize();

		static uint32_t RandomUINT();
		static uint32_t RandomUINT(uint32_t min, uint32_t max);
		static float RandomFloat();
		static glm::vec3 RandomVec3();
		static glm::vec3 RandomVec3(float min, float max);
		static glm::vec3 UnitSphere();

		static uint32_t FastUINT(uint32_t& input);
		static uint32_t FastUINT(uint32_t& input, uint32_t min, uint32_t max);
		static float FastFloat(uint32_t& input);
		static glm::vec3 FastVec3(uint32_t& input);
		static glm::vec3 FastVec3(uint32_t& input, float min, float max);
		static glm::vec3 FastUnitSphere(uint32_t& input);

	private:

		static thread_local std::mt19937 s_RandomEngine;
		static std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;

	};
}