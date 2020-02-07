#pragma once
#include "float3.h"
#include "ray.h"
#include "hittable.h"
#include "random.h"

bool refract(const float3& v, const float3& n, float niOverNt, float3& refracted)
{
	float3 uv = unitVector(v);
	float theta = dot(uv, n);
	float discriminant = 1.0f - niOverNt * niOverNt * (1.0f - theta * theta);
	if (discriminant > 0.0f)
	{
		refracted = niOverNt * (uv - n * theta) - n * sqrt(discriminant);
		return true;
	}
	else
	{
		return false;
	}
}

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
	metal(const float3& albedo, float roughness) : albedo{ albedo }, roughness{ roughness } {}

	virtual bool scatter(const ray& rayIn, const hitRecord& record, float3& attenuation, ray& scattered) const
	{
		float3 reflected = reflect(unitVector(rayIn.direction), record.normal);
		scattered = ray{ record.position, reflected + roughness * randomInUnitSphere()};
		attenuation = albedo;
		return dot(scattered.direction, record.normal) > 0.0f;
	}

	float3 albedo;
	float roughness;
};

class dielectric : public material
{
public:
	dielectric(float refractiveIndex) : refractiveIndex{ refractiveIndex } {}

	virtual bool scatter(const ray& rayIn, const hitRecord& record, float3& attenuation, ray& scattered) const
	{
		float3 outwardNormal;
		float3 reflected = reflect(rayIn.direction, record.normal);
		float niOverNt;
		attenuation = float3::identity;
		float3 refracted;

		if (dot(rayIn.direction, record.normal) > 0.0f)
		{
			outwardNormal = -record.normal;
			niOverNt = refractiveIndex;
		}
		else
		{
			outwardNormal = record.normal;
			niOverNt = 1.0f / refractiveIndex;
		}

		if (refract(rayIn.direction, outwardNormal, niOverNt, refracted))
		{
			scattered = ray{ record.position, refracted };
		}
		else
		{
			scattered = ray{ record.position, reflected };
		}
		return true;
	}

	float refractiveIndex;
};
