#pragma once
#include "float3.h"

class ray
{
public:
	ray(){}
	constexpr ray(const float3& origin, const float3& direction)
		: origin(origin)
		, direction(direction)
		{}

	float3 positionAt(float t) const { return origin + t * direction; }

	float3 origin;
	float3 direction;
};
