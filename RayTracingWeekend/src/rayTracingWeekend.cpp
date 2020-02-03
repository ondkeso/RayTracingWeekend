#include "float3.h"
#include "ray.h"
#include "hittableList.h"
#include "sphere.h"
#include "camera.h"
#include "random.h"

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
	constexpr int samplesPerPixel = 60;

	hittable* list[2];
	list[0] = new sphere(float3{ 0.0f, 0.0f, 1.0f }, 0.5f);
	list[1] = new sphere(float3{ 0.0f, -100.5f, 1.0f }, 100.0f);
	hittable* world = new hittableList{ list, 2 };
	camera cam(float3{ -2.0f, -1.0f, 1.0f },  float3{4.0f, 0.0f, 0.0f} , float3{0.0f, 2.0f, 0.0f}, float3{0.0f, 0.0f, 0.0f});

	std::ofstream cout("output.ppm");
	cout << "P3\n" << nx << " " << ny << "\n255\n";

	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			float3 color{0.0f, 0.0f, 0.0f};

			for (int s = 0; s < samplesPerPixel; ++s)
			{
				const float u = (i + random01()) / float{ nx };
				const float v = (j + random01()) / float{ ny };

				ray r = cam.spawnRay(u, v);
				color += colorOfRay(r, world);
			}
			color /= float{ samplesPerPixel };

			int ir = int(255.9999f * color.r);
			int ig = int(255.9999f * color.g);
			int ib = int(255.9999f * color.b);

			cout << ir << " " << ig << " " << ib << "\n";
		}
	}
	cout.close();

	system(R"("C:\Program Files\IrfanView\i_view64.exe" .\output.ppm /pos=(400,300))");
}
