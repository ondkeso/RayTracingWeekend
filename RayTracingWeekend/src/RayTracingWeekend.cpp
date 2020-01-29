// RayTracingWeekend.cpp : This file contains the 'main' function. Program execution begins and ends there.
constexpr float PI = 3.14159265358979323846f;
constexpr float PI2 = 2.0f * PI;

#include "float3.h"
#include "ray.h"
#include <fstream>

float hit_sphere(const float3& center, float radius, const ray& r)
{
	float3 center_to_origin = r.origin - center;
	float a = dot(r.direction, r.direction);
	float b = 2.0f * dot(center_to_origin, r.direction);
	float c = dot(center_to_origin, center_to_origin) - radius * radius;
	float discriminant = b * b - 4.0f * a * c;

	if (discriminant < 0.0f)
	{
		return -1.0f;
	} 
	else 
	{
		return (-b - sqrt(discriminant)) / (2.0f * a);
	}
}

float3 colorOfRay(const ray& r)
{
	//Test sphere
	{
		const float3 center{ 0.0f, 0.0f, 1.001f };
		float hit = hit_sphere(center, 0.5f, r);
		if (hit > 0.0f)
		{
			float3 normal = unit_vector(r.position_at(hit) - center);
			return 0.5f * float3{ normal.x + 1.0f, normal.y + 1.0f, normal.z + 1.0f };
		}
	}

	float3 unit_direction = unit_vector(r.direction);
	float t = 0.5f * (unit_direction.y + 1.0f);
	return lerp(float3{ 1.0f, 1.0f, 1.0f }, float3{ 0.5f, 0.7f, 1.0f }, t);
}

int main()
{
	constexpr int nx = 400;
	constexpr int ny = 200;
	constexpr float3 lower_left_corner(-2.0f, -1.0f, 1.0f);
	constexpr float3 horizontal(4.0f, 0.0f, 0.0f);
	constexpr float3 vertical(0.0f, 2.0f, 0.0f);
	constexpr float3 origin(0.0f, 0.0f, 0.0f);

	std::ofstream cout("output.ppm");
	cout << "P3\n" << nx << " " << ny << "\n255\n";

	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			float u = (float)i / (float)nx;
			float v = (float)j / (float)ny;

			ray r(origin, lower_left_corner + u * horizontal + v * vertical);

			float3 color = colorOfRay(r);

			int ir = int(255.9999f * color.r);
			int ig = int(255.9999f * color.g);
			int ib = int(255.9999f * color.b);

			cout << ir << " " << ig << " " << ib << "\n";
		}
	}
	cout.close();

	system(R"("C:\Program Files\IrfanView\i_view64.exe" .\output.ppm /pos=(400,300))");
}
