#pragma once

#include "ray.h"

class camera
{
public:
	camera(float3 lowerLeftCorner, float3 horizontal, float3 vertical, float3 origin)
		: lowerLeftCorner {lowerLeftCorner}
		, horizontal {horizontal}
		, vertical {vertical}
		, origin{ origin }
	{}

	ray spawnRay(float u, float v)
	{
		return ray{ origin, lowerLeftCorner + u * horizontal + v * vertical - origin };
	}

	float3 lowerLeftCorner;
	float3 horizontal;
	float3 vertical;
	float3 origin;
};
