#include "float3.h"
#include "ray.h"
#include "hittableList.h"
#include "sphere.h"

#include <limits>
#include <fstream>

float3 colorOfRay(const ray& r, hittable* world)
{
	hitRecord record;
	if (world->hit(r, 0.0f, std::numeric_limits<float>::max(), record))
	{
		return 0.5f * float3{ record.normal.x + 1.0f, record.normal.y + 1.0f, record.normal.z + 1.0f };
	}
	else
	{
		float3 unit_direction = unitVector(r.direction);
		float t = 0.5f * (unit_direction.y + 1.0f);
		return lerp(float3{ 1.0f, 1.0f, 1.0f }, float3{ 0.5f, 0.7f, 1.0f }, t);
	}
}

int main()
{
	constexpr int nx = 400;
	constexpr int ny = 200;
	constexpr float3 lower_left_corner(-2.0f, -1.0f, 1.0f);
	constexpr float3 horizontal(4.0f, 0.0f, 0.0f);
	constexpr float3 vertical(0.0f, 2.0f, 0.0f);
	constexpr float3 origin(0.0f, 0.0f, 0.0f);

	hittable* list[2];
	list[0] = new sphere(float3{ 0.0f, 0.0f, 1.0f }, 0.5f);
	list[1] = new sphere(float3{ 0.0f, -100.5f, 1.0f }, 100.0f);
	hittable* world = new hittableList{ list, 2 };

	std::ofstream cout("output.ppm");
	cout << "P3\n" << nx << " " << ny << "\n255\n";

	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			float u = (float)i / (float)nx;
			float v = (float)j / (float)ny;

			ray r(origin, lower_left_corner + u * horizontal + v * vertical);

			float3 color = colorOfRay(r, world);

			int ir = int(255.9999f * color.r);
			int ig = int(255.9999f * color.g);
			int ib = int(255.9999f * color.b);

			cout << ir << " " << ig << " " << ib << "\n";
		}
	}
	cout.close();

	system(R"("C:\Program Files\IrfanView\i_view64.exe" .\output.ppm /pos=(400,300))");
}
