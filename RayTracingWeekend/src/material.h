#pragma once
#include "float3.h"
#include "ray.h"
#include "hittable.h"
#include "random.h"
#include "mathConstants.h"
#include <stdio.h>

// Returns true if the incidentRay is potentially transmitted
bool refract(const float3& incidentRay, const float3& normalFacingIncidentRay, float cosIncidentAngle, float niOverNt, float3* const refractedOut)
{
	const float radicand = 1.0f - niOverNt * niOverNt * (1.0f - cosIncidentAngle * cosIncidentAngle);
	if (radicand > 0.0f)
	{
		//Potentially transmitted
		*refractedOut = niOverNt * incidentRay + (niOverNt * cosIncidentAngle - sqrtf(radicand)) * normalFacingIncidentRay;
		return true;
	}
	else
	{
		//Total internal reflection occurs
		return false;
	}
}

float schlick(float cosSchlickAngle, float ni, float nt)
{
	const float sqrtOfr0 = (ni - nt) / (ni + nt);
	const float r0 = sqrtOfr0 * sqrtOfr0;
	const float powFactor = 1.0f - cosSchlickAngle;
	return r0 + (1.0f - r0) * powFactor * powFactor * powFactor * powFactor * powFactor;
}

class material
{
public:
	virtual bool transmit(const ray& rayIn, const hitRecord& record, float3* const attenuation, ray* const transmittedOut) const = 0;
};

class lambertian : public material
{
public:
	lambertian(const float3& albedo) : albedo{ albedo } {}

	virtual bool transmit(const ray& rayIn, const hitRecord& record, float3* const attenuation, ray* const transmittedOut) const
	{
		float3 scatterDirection = record.position + record.normal + randomOnUnitSphere();
		*transmittedOut = ray{ record.position, scatterDirection - record.position };
		*attenuation = albedo;
		return true;
	}

	float3 albedo;
};

class metal : public material
{
public:
	metal(const float3& albedo, float roughness) : albedo{ albedo }, roughness{ roughness } {}

	virtual bool transmit(const ray& rayIn, const hitRecord& record, float3* const attenuation, ray* const transmittedOut) const
	{
		float3 reflected = reflect(unitVector(rayIn.direction), record.normal);
		*transmittedOut = ray{ record.position, reflected + roughness * randomInUnitSphere()};
		*attenuation = albedo;
		return dot(transmittedOut->direction, record.normal) > 0.0f;
	}

	float3 albedo;
	float roughness;
};

class dielectric : public material
{
public:
	dielectric(float refractiveIndex) : refractiveIndex{ refractiveIndex } {}

	virtual bool transmit(const ray& rayIn, const hitRecord& record, float3* const attenuation, ray* const transmittedOut) const
	{
		*attenuation = float3::identity();
		transmittedOut->origin = record.position;

		const float3 rayInUnitDirection = unitVector(rayIn.direction);
		const float rayInDotNormal = dot(rayInUnitDirection, record.normal);

		float3 normalFacingIncidentRay;
		float cosIncidentAngle, ni, nt;

		const bool hitFromOutside = rayInDotNormal < 0.0f;
		if (hitFromOutside)
		{
			//Probe ray is hitting this material from the outside
			cosIncidentAngle = -rayInDotNormal;
			normalFacingIncidentRay = record.normal;
			ni = 1.0f;
			nt = refractiveIndex;
		}
		else
		{
			//Probe ray is hitting material from the inside
			cosIncidentAngle = rayInDotNormal;
			normalFacingIncidentRay = -record.normal;
			ni = refractiveIndex;
			nt = 1.0f;
		}

		const bool can_refract = refract(rayInUnitDirection, normalFacingIncidentRay, cosIncidentAngle, ni / nt, &transmittedOut->direction);

		if (can_refract)
		{
			// If ni > nt, we must use angle of transmittance for Schlick
			//Reference https://graphics.stanford.edu/courses/cs148-10-summer/docs/2006--degreve--reflection_refraction.pdf
			const float cosSchlickAngle = ni > nt ? dot(transmittedOut->direction, record.normal) : cosIncidentAngle;

			//Refracted. Refraction direction already calculated above
			if (random01() > schlick(cosSchlickAngle, ni, nt))
				return true;
		}

		//Reflect
		transmittedOut->direction = reflect(rayInUnitDirection, record.normal, rayInDotNormal);
		return true;
	}

	float refractiveIndex;
};
