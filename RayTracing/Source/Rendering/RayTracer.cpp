/// \ project: RayTracing
/// \ file: RayTracer.cpp
/// \ author: Thomas Clermont

#include "RayTracer.h"
#include <Utils/Random.h>
#include <execution>



namespace Utils
{
	static uint32_t ConvertToRGBA(const glm::vec4& color)
	{
		uint8_t r = (uint8_t)(color.r * 255.0f);
		uint8_t g = (uint8_t)(color.g * 255.0f);
		uint8_t b = (uint8_t)(color.b * 255.0f);
		uint8_t a = (uint8_t)(color.a * 255.0f);
		uint32_t result = (a << 24) | (b << 16) | (g << 8) | r;
		return result;
	}

}




void RayTracer::Render(const Camera& camera, const Scene& scene)
{
	m_ActiveCamera = &camera;
	m_ActiveScene = &scene;

	if (m_FrameIndex == 1)
		memset(m_AccumulationData, 0, m_Image->GetWidth() *
			m_Image->GetHeight() * sizeof(glm::vec4));


#define MT 1
#if MT

	std::for_each(std::execution::par, m_ImageVerticalIter.begin(), m_ImageVerticalIter.end(),
		[this](uint32_t y)
		{
			std::for_each(std::execution::par, m_ImageHorizontalIter.begin(), m_ImageHorizontalIter.end(),
				[this, y](uint32_t x)
				{
					glm::vec4 color = PerPixel(x, y);
					m_AccumulationData[x + y * m_Image->GetWidth()] += color;

					glm::vec4 accumulatedColor = m_AccumulationData[x + y * m_Image->GetWidth()];
					accumulatedColor /= (float)m_FrameIndex;

					if (accumulatedColor.r < 0.0f) accumulatedColor.r = 0.0f;
					if (accumulatedColor.g < 0.0f) accumulatedColor.g = 0.0f;
					if (accumulatedColor.b < 0.0f) accumulatedColor.b = 0.0f;
					if (accumulatedColor.a < 0.0f) accumulatedColor.a = 0.0f;
					if (accumulatedColor.r > 1.0f) accumulatedColor.r = 1.0f;
					if (accumulatedColor.g > 1.0f) accumulatedColor.g = 1.0f;
					if (accumulatedColor.b > 1.0f) accumulatedColor.b = 1.0f;
					if (accumulatedColor.a > 1.0f) accumulatedColor.a = 1.0f;

					m_ImageData[x + y * m_Image->GetWidth()] = Utils::ConvertToRGBA(accumulatedColor);
				});
		});

#else

	for (uint32_t y = 0; y < m_Image->GetHeight(); y++)
	{
		for (uint32_t x = 0; x < m_Image->GetWidth(); x++)
		{
			glm::vec4 color = PerPixel(x, y);
			m_AccumulationData[x + y * m_Image->GetWidth()] += color;

			glm::vec4 accumulatedColor = m_AccumulationData[x + y * m_Image->GetWidth()];
			accumulatedColor /= (float)m_FrameIndex;

			if (accumulatedColor.r < 0.0f) accumulatedColor.r = 0.0f;
			if (accumulatedColor.g < 0.0f) accumulatedColor.g = 0.0f;
			if (accumulatedColor.b < 0.0f) accumulatedColor.b = 0.0f;
			if (accumulatedColor.a < 0.0f) accumulatedColor.a = 0.0f;
			if (accumulatedColor.r > 1.0f) accumulatedColor.r = 1.0f;
			if (accumulatedColor.g > 1.0f) accumulatedColor.g = 1.0f;
			if (accumulatedColor.b > 1.0f) accumulatedColor.b = 1.0f;
			if (accumulatedColor.a > 1.0f) accumulatedColor.a = 1.0f;

			m_ImageData[x + y * m_Image->GetWidth()] = Utils::ConvertToRGBA(accumulatedColor);
		}
	}

#endif

	if (m_Settings.Accumulate)
		m_FrameIndex++;
	else
		m_FrameIndex = 1;

	m_Image->SetData(m_ImageData);
}

glm::vec4 RayTracer::PerPixel(uint32_t x, uint32_t y)
{
	Ray ray;
	ray.Origin = m_ActiveCamera->GetPosition();
	ray.Direction = m_ActiveCamera->GetRayDirections()[x + y * m_Image->GetWidth()];

	glm::vec3 light(0.0f);
	glm::vec3 color(1.0f);

	for (int i = 0; i < m_Settings.Bounces; i++)
	{
		HitPayload payload = TraceRay(ray);
		if (payload.HitDistance < 0.0f)
		{
			glm::vec3 skyColor(0.6f, 0.7f, 1.0f);
			light += skyColor * color;
			break;
		}

		const Sphere& sphere = m_ActiveScene->Spheres[payload.ObjectIndex];
		const Material& material = m_ActiveScene->Materials[sphere.MaterialIndex];

		color *= material.Albedo;
		light += material.GetEmission();

		ray.Origin = payload.WorldPosition + payload.WorldNormal * 0.0001f;
		ray.Direction = glm::normalize(payload.WorldNormal + SNG::Random::UnitSphere());
	}

	return glm::vec4(light, 1.0f);
}

HitPayload RayTracer::TraceRay(const Ray& ray)
{
	int closestSphere = -1;
	float hitDistance = FLT_MAX;

	for (size_t i = 0; i < m_ActiveScene->Spheres.size(); i++)
	{
		const Sphere& sphere = m_ActiveScene->Spheres[i];
		glm::vec3 origin = ray.Origin - sphere.Position;

		float a = glm::dot(ray.Direction, ray.Direction);
		float b = 2.0f * glm::dot(origin, ray.Direction);
		float c = glm::dot(origin, origin) - (sphere.Radius * sphere.Radius);

		// Quadratic formula discrimiant: b^2-4ac
		float discriminant = b * b - 4.0f * a * c;
		if (discriminant < 0.0f)
			continue;

		// (-b +- sqrt(discrimiant)) / 2a
		float closestT = (-b - glm::sqrt(discriminant)) / 2.0f * a;
		if (closestT > 0.0f && closestT < hitDistance)
		{
			hitDistance = closestT;
			closestSphere = (int)i;
		}
	}

	if (closestSphere < 0)
		return Miss(ray);

	return ClosestHit(ray, hitDistance, closestSphere);
}

HitPayload RayTracer::ClosestHit(const Ray& ray, float hitDistance, int objectIndex)
{
	HitPayload payload;
	payload.HitDistance = hitDistance;
	payload.ObjectIndex = objectIndex;

	const Sphere& closestSphere = m_ActiveScene->Spheres[objectIndex];

	glm::vec3 origin = ray.Origin - closestSphere.Position;
	payload.WorldPosition = origin + ray.Direction * hitDistance;
	payload.WorldNormal = glm::normalize(payload.WorldPosition);
	payload.WorldPosition += closestSphere.Position;

	return payload;
}

HitPayload RayTracer::Miss(const Ray& ray)
{
	HitPayload payload{};
	payload.HitDistance = -1.0f;
	return payload;
}

void RayTracer::OnResize(uint32_t viewportWidth, uint32_t viewportHeight)
{
	if (m_Image)
	{
		if (viewportWidth == m_Image->GetWidth() && viewportHeight == m_Image->GetHeight())
			return;

		m_Image->Resize(viewportWidth, viewportHeight);
	}
	else
	{
		m_Image = std::make_shared<SNG::Image>(viewportWidth, viewportHeight);
	}

	delete[] m_ImageData;
	m_ImageData = new uint32_t[viewportWidth * viewportHeight];
	delete[] m_AccumulationData;
	m_AccumulationData = new glm::vec4[viewportWidth * viewportHeight];
	m_FrameIndex = 1;

	m_ImageHorizontalIter.resize(viewportWidth);
	m_ImageVerticalIter.resize(viewportHeight);
	for (uint32_t i = 0; i < viewportWidth; i++)
		m_ImageHorizontalIter[i] = i;
	for (uint32_t i = 0; i < viewportHeight; i++)
		m_ImageVerticalIter[i] = i;
}
