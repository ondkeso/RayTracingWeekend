#pragma once
#include "float3.h"
#include "ray.h"
#include "hittable.h"
#include "random.h"

class material
{
public:
	virtual bool scatter(const ray& rayIn, const hitRecord& record, float3& attenuation, ray& scattered) const = 0;
};

class lambertian : public material
{
public:
	lambertian(const float3& albedo) : albedo{ albedo } {}

	virtual bool scatter(const ray& rayIn, const hitRecord& record, float3& attenuation, ray& scattered) const
	{
		float3 scatterDirection = record.position + record.normal + randomInUnitSphere();
		scattered = ray{ record.position, scatterDirection - record.position };
		attenuation = albedo;
		return true;
	}

	float3 albedo;
};

class metal : public material
{
public:
	metal(const float3& albedo) : albedo{ albedo } {}

	virtual bool scatter(const ray& rayIn, const hitRecord& record, float3& attenuation, ray& scattered) const
	{
		float3 reflected = reflect(unitVector(rayIn.direction), record.normal);
		scattered = ray{ record.position, reflected };
		attenuation = albedo;
		return dot(scattered.direction, record.normal) > 0.0f;
	}

	float3 albedo;
};
