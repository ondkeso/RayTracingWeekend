#pragma once
#include "hittable.h"

class sphere : public hittable
{
public:
	sphere() {}
	sphere(float3 center, float radius) 
		: center{ center }
		, radius{ radius } {}

	virtual bool hit(const ray& r, float tMin, float tMax, hitRecord& record) const override;

	float3 center;
	float radius;
};

bool sphere::hit(const ray& r, float tMin, float tMax, hitRecord& record) const
{
	const float3 center_to_ray_origin = r.origin - center;
	const float a = dot(r.direction, r.direction);
	const float b = dot(center_to_ray_origin, r.direction);
	const float c = dot(center_to_ray_origin, center_to_ray_origin) - radius * radius;
	const float discriminant = b*b - a*c;
	
	if (discriminant > 0.0f)
	{
		float temp = (-b - sqrt(discriminant)) / a;
		if (temp < tMax && temp > tMin)
		{
			record.t = temp;
			record.position = r.positionAt(record.t);
			record.normal = (record.position - center) / radius;
			return true;
		}

		temp = (-b + sqrt(discriminant)) / a;
		if (temp < tMax && temp > tMin)
		{
			record.t = temp;
			record.position = r.positionAt(record.t);
			record.normal = (record.position - center) / radius;
			return true;
		}
	}
	return false;
}
