#include <math.h>
#include <stdlib.h>
#include <iostream>

#pragma warning(push)
#pragma warning(disable : 26495)

class float3 {
public:

	float3() {}
	explicit float3(float e0, float e1, float e2) { e[0] = e0; e[1] = e1; e[2] = e2; }

	inline const float3& operator+() const { return *this; }
	inline float3 operator-() const { return float3{ -e[0], -e[1], -e[2] }; }
	inline float operator[](int i) const { return e[i]; }
	inline float& operator[](int i) { return e[i]; }

	inline float3& operator+=(const float3& v);
	inline float3& operator-=(const float3& v);
	inline float3& operator*=(const float3& v);
	inline float3& operator/=(const float3& v);

	inline float3& operator+=(float f);
	inline float3& operator-=(float f);
	inline float3& operator*=(float f);
	inline float3& operator/=(float f);

	inline float length() const;
	inline float squared_length() const;
	inline void normalize();

	union {
		float e[3];
		struct { float x, y, z; };
		struct { float r, g, b; };
	};
};

#pragma warning(pop)

#pragma region operators
inline float3& float3::operator+=(const float3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

inline float3& float3::operator-=(const float3& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

inline float3& float3::operator*=(const float3& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	return *this;
}

inline float3& float3::operator/=(const float3& v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;
	return *this;
}

inline float3& float3::operator+=(float f)
{
	x += f;
	y += f;
	z += f;
	return *this;
}

inline float3& float3::operator-=(float f)
{
	x -= f;
	y -= f;
	z -= f;
	return *this;
}

inline float3& float3::operator*=(float f)
{
	x *= f;
	y *= f;
	z *= f;
	return *this;
}

inline float3& float3::operator/=(float f)
{
	const float k = 1.0f / f;
	x *= k;
	y *= k;
	z *= k;
	return *this;
}

inline float3 operator+(const float3& v0, const float3& v1)
{
	return float3{ v0.x + v1.x, v0.y + v1.y, v0.z + v1.z };
}

inline float3 operator-(const float3& v0, const float3& v1)
{
	return float3{ v0.x - v1.x, v0.y - v1.y, v0.z - v1.z };
}

inline float3 operator*(const float3& v0, const float3& v1)
{
	return float3{ v0.x * v1.x, v0.y * v1.y, v0.z * v1.z };
}

inline float3 operator/(const float3& v0, const float3& v1)
{
	return float3{ v0.x / v1.x, v0.y / v1.y, v0.z / v1.z };
}

inline float3 operator*(float f, const float3& v)
{
	return float3{ v.x * f, v.y * f, v.z * f };
}

inline float3 operator/(const float3& v, float f)
{
	return float3{ v.x / f, v.y / f, v.z / f };
}
#pragma endregion operators

inline float float3::length() const
{
	return sqrt(squared_length());
}	

inline float float3::squared_length() const
{
	return x * x + y * y + z * z;
}

inline void float3::normalize()
{
	(*this) /= length();
}

inline float3 unit_vector(const float3& v)
{
	return v / v.length();
}

inline float dot(const float3& v0, const float3& v1)
{
	return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z;
}

inline float3 cross(const float3& a, const float3& b)
{
	return float3{
		a[1]*b[2] - a[2]*b[1],
		a[2]*b[0] - a[0]*b[2],
		a[0]*b[1] - a[1]*b[0]
	};
}
