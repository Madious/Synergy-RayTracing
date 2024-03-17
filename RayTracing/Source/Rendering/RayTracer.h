/// \ project: RayTracing
/// \ file: RayTracer.h
/// \ author: Thomas Clermont

#pragma once
#include "Camera.h"
#include "Scene.h"
#include "Ray.h"
#include <Rendering/Image.h>


struct RayTracerSettings
{
	bool Accumulate = true;
	int Bounces = 5;
};

class RayTracer
{
public:

	RayTracer() = default;

	void Render(const Camera& camera, const Scene& scene);
	void OnResize(uint32_t viewportWidth, uint32_t viewportHeight);

	inline void ResetFrameIndex() { m_FrameIndex = 1; }
	inline uint32_t GetFrameIndex() const { return m_FrameIndex; }
	inline std::shared_ptr<SNG::Image> GetImage() const { return m_Image; }
	inline RayTracerSettings& GetSettings() { return m_Settings; }

private:

	glm::vec4 PerPixel(uint32_t x, uint32_t y);
	HitPayload TraceRay(const Ray& ray);
	HitPayload Miss(const Ray& ray);
	HitPayload ClosestHit(const Ray& ray, float hitDistance, int objectIndex);


private:

	std::shared_ptr<SNG::Image> m_Image = nullptr;
	const Scene* m_ActiveScene = nullptr;
	const Camera* m_ActiveCamera = nullptr;

	RayTracerSettings m_Settings;
	std::vector<uint32_t> m_ImageHorizontalIter, m_ImageVerticalIter;

	uint32_t* m_ImageData = nullptr;
	glm::vec4* m_AccumulationData = nullptr;
	uint32_t m_FrameIndex = 1;

};
