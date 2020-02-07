#include "float3.h"
#include "ray.h"
#include "hittableList.h"
#include "sphere.h"
#include "camera.h"
#include "random.h"
#include "material.h"

#include <limits>
#include <fstream>

float3 colorOfRay(const ray& r, hittable* world, int currentRecursion = 0)
{
	hitRecord record;
	if (world->hit(r, 0.0001f, std::numeric_limits<float>::max(), record))
	{
		ray scattered;
		float3 attenuation;
		if (currentRecursion < 50 && record.material->scatter(r, record, attenuation, scattered))
		{
			return attenuation * colorOfRay(scattered, world, currentRecursion + 1);
		}
		else
		{
			return float3::zero;
		}
	}
	else
	{
		float3 unitDirection = unitVector(r.direction);
		float t = 0.5f * (unitDirection.y + 1.0f);
		return lerp(float3::identity, float3{ 0.5f, 0.7f, 1.0f }, t);
	}
}

int main()
{
	constexpr int nx = 500;
	constexpr int ny = 250;
	constexpr int samplesPerPixel = 50;

	hittable* list[4];
	list[0] = new sphere{ float3{0.0f, 0.0f, 1.0f}, 0.5f, new lambertian{float3{0.8f, 0.3f, 0.3f }} };
	list[1] = new sphere{ float3{0.0f, -100.5f, 1.0f}, 100, new lambertian{float3{0.8f, 0.8f, 0.0f }} };
	list[2] = new sphere{ float3{1.0f, 0.0f, 1.0f}, 0.5f, new metal{float3{0.8f, 0.6f, 0.2f }} };
	list[3] = new sphere{ float3{-1.0f, 0.0f, 1.0f}, 0.5f, new metal{float3{0.8f, 0.8f, 0.8f }} };

	hittable* world = new hittableList{ list, 4 };
	camera cam(float3{ -2.0f, -1.0f, 1.0f },  float3{4.0f, 0.0f, 0.0f} , float3{0.0f, 2.0f, 0.0f}, float3::zero);

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

			//Gamma correct
			color = float3{ sqrt(color.r), sqrt(color.g), sqrt(color.b) };

			int ir = int(255.9999f * color.r);
			int ig = int(255.9999f * color.g);
			int ib = int(255.9999f * color.b);

			cout << ir << " " << ig << " " << ib << "\n";
		}
	}
	cout.close();

	system(R"("C:\Program Files\IrfanView\i_view64.exe" .\output.ppm /pos=(400,300))");
}
