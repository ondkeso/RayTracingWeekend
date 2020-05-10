#pragma once
#include "ray.h"
#include "mathConstants.h"

class camera
{
public:
	camera(float verticalFovDegrees, float aspectRatio)
	{
		const float vFovRad = verticalFovDegrees * DEG_TO_RAD;
		const float halfHeight = tanf(vFovRad * 0.5f);
		const float halfWidth = aspectRatio * halfHeight;

		lowerLeftCorner = float3{ -halfWidth, -halfHeight, 1.0f };
		horizontal = float3{ 2.0f * halfWidth, 0.0f, 0.0f };
		vertical = float3{ 0.0f, 2.0f * halfHeight, 0.0f };
	}

	constexpr ray spawnRay(float u, float v) const
	{
		return ray{ origin, lowerLeftCorner + u * horizontal + v * vertical - origin };
	}

private:
	float3 origin = float3::zero();
	float3 lowerLeftCorner;
	float3 horizontal;
	float3 vertical;
};
