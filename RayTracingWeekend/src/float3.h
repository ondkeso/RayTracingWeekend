#include <math.h>
#include <stdlib.h>
#include <iostream>

class float3 {
public:

	float3() {}
	float3(float e0, float e1, float e2) { e[0] = e0; e[1] = e1; e[2] = e2; }

	inline const float3& operator+() const { return *this; }
	inline float3 operator-() const { return float3(-e[0], -e[1], -e[2]); }
	inline float operator[](int i) const { return e[i]; }
	inline float& operator[](int i) { return e[i]; }

	inline float3& operator+=(const float3& v);
	inline float3& operator-=(const float3& v);
	inline float3& operator*=(const float3& v);
	inline float3& operator/=(const float3& v);

	inline float3& operator+=(float f) const;
	inline float3& operator-=(float f) const;
	inline float3& operator*=(float f) const;
	inline float3& operator/=(float f) const;

	inline float length() const;
	inline float squared_length() const;
	inline float normalize();

	union {
		float e[3];
		struct { float x, y, z; };
		struct { float r, g, b; };
	};
};

inline float3& float3::operator+=(const float3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
}

inline float3& float3::operator-=(const float3& v) 
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
}

inline float3& float3::operator*=(const float3& v) 
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
}

inline float3& float3::operator/=(const float3& v) 
{
	x /= v.x;
	y /= v.y;
	z /= v.z;
}

inline float3& float3::operator+=(float f) const
{

}
inline float3& float3::operator-=(float f) const
{

}
inline float3& float3::operator*=(float f) const
{

}
inline float3& float3::operator/=(float f) const
{

}

inline float float3::length() const
{
	return sqrt(x * x + y * y + z * z);
}	

inline float float3::squared_length() const
{
	return x * x + y * y + z * z;
}

inline float float3::normalize()
{
	float k = 1.0f / length();
	(*this) *= k;
}
