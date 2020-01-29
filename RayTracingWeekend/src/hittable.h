#pragma once

#include "ray.h"

struct hitRecord
{
	float t;
	float3 position;
	float3 normal;
};

class hittable
{
public:
	virtual bool hit(const ray& r, float t_min, float t_max, hitRecord& record) const = 0;
};