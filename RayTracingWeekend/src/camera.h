#pragma once
#include "ray.h"
#include "mathConstants.h"

class camera
{
public:
	camera(float3 lookFrom, float3 lookAt, float3 viewUp, float verticalFovDegrees, float aspectRatio)
	{
		const float vFovRad = verticalFovDegrees * DEG_TO_RAD;
		const float halfHeight = tanf(vFovRad * 0.5f);
		const float halfWidth = aspectRatio * halfHeight;
		const float3 w = unitVector(lookFrom - lookAt);
		const float3 u = unitVector(cross(viewUp, w));
		const float3 v = cross(w, u);

		origin = lookFrom;
		lowerLeftCorner = origin - halfWidth * u - halfHeight * v - w;
		horizontal = 2.0f * halfWidth * u;
		vertical = 2.0f * halfHeight * v;
	}

	constexpr ray spawnRay(float u, float v) const
	{
		return ray{ origin, lowerLeftCorner + u * horizontal + v * vertical - origin };
	}

private:
	float3 origin;
	float3 lowerLeftCorner;
	float3 horizontal;
	float3 vertical;
};
