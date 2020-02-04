#pragma once
#include "float3.h"
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

inline float3 randomInUnitSphere()
{
	float3 p;
	do
	{
		p = 2.0f * float3{ random01(), random01(), random01() } -float3{ 1.0f, 1.0f, 1.0f };
	} while (p.squaredLength() >= 1.0f);
	return p;
}
