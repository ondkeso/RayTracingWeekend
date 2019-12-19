// RayTracingWeekend.cpp : This file contains the 'main' function. Program execution begins and ends there.
constexpr float PI = 3.14159265358979323846f;
constexpr float PI2 = 2.0f * PI;

#include "float3.h"
#include "ray.h"
#include <fstream>

float3 color(const ray& r)
{
	float3 unit_direction = unit_vector(r.direction);
	float t = 0.5f * (unit_direction.y + 1.0f);
	return lerp(float3(1.0f, 1.0f, 1.0f), float3(0.5f, 0.7f, 1.0f), t);
}

int main()
{
	std::ofstream cout("output.ppm");

	int nx = 300;
	int ny = 200;
	cout << "P3\n" << nx << " " << ny << "\n255\n";

	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			float3 color{ sin(3.0f * PI2 * float(i) / float(nx))* 0.5f + 0.5f, sin(2.0f * PI2 * float(j) / float(ny)) * 0.5f + 0.5f, 0.01f };

			color.normalize();

			int ir = int(255.9999f * pow(color.r, 2.0f));
			int ig = int(255.9999f * pow(color.g, 2.0f));
			int ib = int(255.9999f * pow(color.b, 2.0f));

			cout << ir << " " << ig << " " << ib << "\n";
		}
	}
	cout.close();

	system(R"("C:\Program Files\IrfanView\i_view64.exe" .\output.ppm /pos=(400,300))");
}
