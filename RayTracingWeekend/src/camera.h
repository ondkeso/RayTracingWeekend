#pragma once
#include "ray.h"
#include "mathConstants.h"
#include "random.h"

class camera
{
public:
	camera(float3 lookFrom, float3 lookAt, float3 viewUp, float verticalFovDegrees, float aspectRatio
		, float aperatureRadius, float focusDistance)
		: origin{ lookFrom }
		, w{ unitVector(lookFrom - lookAt) }
		, u{ unitVector(cross(viewUp, w)) }
		, v{ cross(w, u) }
		, lensRadius{ aperatureRadius }
	{
		const float vFovRad = verticalFovDegrees * DEG_TO_RAD;
		const float halfHeight = tanf(vFovRad * 0.5f);
		const float halfWidth = aspectRatio * halfHeight;

		lowerLeftCorner = origin
			- halfWidth * focusDistance * u
			- halfHeight * focusDistance * v
			- focusDistance * w;
		horizontal = 2.0f * halfWidth * focusDistance * u;
		vertical = 2.0f * halfHeight * focusDistance * v;
	}

	ray spawnRay(float u, float v) const
	{
		const float3 randomPointOnLens{ lensRadius * randomInUnitDisk() };
		const float3 offset = this->u * randomPointOnLens.x + this->v * randomPointOnLens.y;
		return ray{ origin + offset
			, lowerLeftCorner + u * horizontal + v * vertical - origin - offset};
	}

private:
	float3 origin;
	float3 w, u, v;
	float lensRadius;
	float3 lowerLeftCorner;
	float3 horizontal;
	float3 vertical;
};
