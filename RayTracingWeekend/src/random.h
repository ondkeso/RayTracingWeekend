#pragma once
#include "float3.h"
#include "mathConstants.h"
#include <inttypes.h>
#include <limits>

inline uint32_t xor128()
{
	static uint32_t x = 123456789;
	static uint32_t y = 362436069;
	static uint32_t z = 521288629;
	static uint32_t w = 88675123;
	uint32_t t;
	t = x ^ (x << 11);
	x = y; y = z; z = w;
	return w = w ^ (w >> 19) ^ (t ^ (t >> 8));
}

inline float random01()
{
	return float(xor128()) / float(std::numeric_limits<uint32_t>::max());
}

inline float random(float min, float max)
{
	return random01() * (max - min) + min;
}

//Unit sphere picking reference https://mathworld.wolfram.com/SpherePointPicking.html
inline float3 randomOnUnitSphere()
{
	const float a = random(0.0f, PI2);
	const float z = random(-1.0f, 1.0f);
	const float r = sqrtf(1.0f - z * z);
	return float3{ r*cos(a), r*sin(a), z };
}

inline float3 randomOnUnitSphere2()
{
	float x, y, r;
	float xsq, ysq;
	do
	{
		x = random(-1.0f, 1.0f);
		y = random(-1.0f, 1.0f);

		xsq = x * x;
		ysq = y * y;

		r = xsq + ysq;
	} while (r >= 1.0f);

	const float k = 2.0f * sqrtf(1.0f - xsq - ysq);

	return float3{ x * k, y * k, 1.0f - 2.0f * r };
}

inline float3 randomOnUnitSphere3()
{
	float x0, x1, x2, x3;
	float x0sq, x1sq, x2sq, x3sq;
	float divisor;

	do
	{
		//It would be very nice to vectorize this process
		x0 = random(-1.0f, 1.0f);
		x1 = random(-1.0f, 1.0f);
		x2 = random(-1.0f, 1.0f);
		x3 = random(-1.0f, 1.0f);

		x0sq = x0 * x0;
		x1sq = x1 * x1;
		x2sq = x2 * x2;
		x3sq = x3 * x3;

		divisor = x0sq + x1sq + x2sq + x3sq;

	} while(divisor >= 1.0f);

	const float xyScaler = 2.0f / divisor;

	return float3{	xyScaler * (x1*x3 + x0*x2),
					xyScaler * (x2*x3 - x0*x1),
					(x0sq + x3sq - x1sq - x2sq) / divisor };
}

inline float3 randomInUnitSphere()
{
	float3 p;
	do
	{
		p = 2.0f * float3{ random01(), random01(), random01() } - float3::identity();
	} while (p.squaredLength() >= 1.0f);
	return p;
}
