#pragma once
#include "hittable.h"

class material;

class sphere : public hittable
{
public:
	sphere() {}
	sphere(float3 center, float radius, const material* material)
		: center{ center }
		, radius{ radius }
		, material{ material } {}

	virtual bool hit(const ray& r, float tMin, float tMax, hitRecord& record) const override;

	float3 center;
	float radius;
	const material* material;
};

bool sphere::hit(const ray& r, float tMin, float tMax, hitRecord& record) const
{
	const float3 centerToRayOrigin = r.origin - center;
	const float a = r.direction.squaredLength();
	const float halfB = dot(centerToRayOrigin, r.direction);
	const float c = centerToRayOrigin.squaredLength() - radius * radius;
	const float discriminant = halfB * halfB - a*c;
	
	if (discriminant > 0.0f)
	{
		const float root = sqrtf(discriminant);
		float temp = (-halfB - root) / a;
		if (temp < tMax && temp > tMin)
		{
			record.t = temp;
			record.position = r.positionAt(record.t);
			record.normal = (record.position - center) / radius;
			record.material = material;
			return true;
		}

		temp = (-halfB + root) / a;
		if (temp < tMax && temp > tMin)
		{
			record.t = temp;
			record.position = r.positionAt(record.t);
			record.normal = (record.position - center) / radius;
			record.material = material;
			return true;
		}
	}
	return false;
}
