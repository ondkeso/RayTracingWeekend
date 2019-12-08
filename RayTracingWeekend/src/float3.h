#include <math.h>
#include <stdlib.h>
#include <iostream>

class float3 {
public:

	float3() {}
	float3(float e0, float e1, float e2) { e[0] = e0; e[1] = e1; e[2] = e2; }

	inline const float3& operator+() const { return *this; }
	inline float3 operator-() const { return float3(-x, -y, -z); }
	inline float operator[](int i) const { return e[i]; }
	inline float& operator[](int i) { return e[i]; }


	union {
		float e[3];
		struct { float x, y, z; };
		struct { float r, g, b; };
	};
};