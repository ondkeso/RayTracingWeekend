#pragma once
#include "float3.h"

class ray
{
public:
	ray(){}
	ray(const float3& origin, const float3& direction)
		: origin(origin)
		, direction(direction)
		{}

	float3 position_at(float t) const { return origin + t * direction; }

	float3 origin;
	float3 direction;
};
