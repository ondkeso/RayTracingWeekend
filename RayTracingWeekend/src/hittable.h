#pragma once
#include "ray.h"

class material;

struct hitRecord
{
	float t;
	float3 position;
	float3 normal;
	const material* material;
};

class hittable
{
public:
	virtual bool hit(const ray& r, float tMin, float tMax, hitRecord& record) const = 0;
};
