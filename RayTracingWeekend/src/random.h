#pragma once

#include <cstdlib>

inline float random01()
{
	return rand() / (RAND_MAX + 1.0f);
}
