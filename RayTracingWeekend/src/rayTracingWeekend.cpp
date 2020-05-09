#include "float3.h"
#include "ray.h"
#include "hittableList.h"
#include "sphere.h"
#include "camera.h"
#include "random.h"
#include "material.h"

#include <limits>
#include <fstream>
#include <vector>

float3 colorOfRay(const ray& r, const hittable& hittable, int currentRecursion = 0)
{
	hitRecord record;
	if (hittable.hit(r, 0.001f, std::numeric_limits<float>::max(), record))
	{
		ray transmitted;
		float3 attenuation;
		if (currentRecursion < 20 && record.material->transmit(r, record, &attenuation, &transmitted))
		{
			return attenuation * colorOfRay(transmitted, hittable, currentRecursion + 1);
		}
		else
		{
			return float3::zero();
		}
	}
	else
	{
		float3 unitDirection = unitVector(r.direction);
		float t = 0.5f * (unitDirection.y + 1.0f);
		return lerp(float3::identity(), float3{ 0.5f, 0.7f, 1.0f }, t);
	}
}

int main()
{
	constexpr float qualityFactor = 1.0f;
	constexpr int nx = (int) (500 * qualityFactor);
	constexpr int ny = (int) (250 * qualityFactor);
	constexpr int samplesPerPixel = (int) (20 * qualityFactor);

	std::vector<hittable*> sphereScene;
	//hittables.emplace_back(sphere{ float3{0.0f, 0.0f, 1.7f}, 0.5f, new lambertian{float3{0.1f, 0.2f, 0.5f }} });
	//hittables.emplace_back(sphere{ float3{0.0f, -100.5f, 1.7f}, 100, new lambertian{float3{0.8f, 0.8f, 0.0f }} });
	//hittables.emplace_back(sphere{ float3{1.0f, 0.0f, 1.7f}, 0.5f, new metal{float3{0.8f, 0.6f, 0.2f }, 0.0f} });
	//hittables.emplace_back(sphere{ float3{-1.0f, 0.0f, 1.2f}, 0.5f, new dielectric{1.3f} });
	//hittables.emplace_back(sphere{ float3{1.2f, 0.0f, 1.2f}, 0.5f, new dielectric{0.7f} });

	float x = -4.5f;
	for (int i = 0; i < 10; ++i)
	{
		sphereScene.emplace_back(new sphere{ float3{x++, 0.9f, -2.2f}, 0.5f, new lambertian{float3{0.01f, 0.01f, 0.01f }} });
	}
	sphereScene.emplace_back(new sphere{ float3{0.0f, -100.9f, 1.0f}, 100, new lambertian{float3{0.8f, 0.8f, 0.0f }} });
	sphereScene.emplace_back(new sphere{ float3{0.0f, 0.0f, 1.0f}, 0.5f, new dielectric{1.5f} });

	const hittableList world{ sphereScene };
	constexpr camera cam(float3{ -2.0f, -1.0f, 1.0f },  float3{4.0f, 0.0f, 0.0f} , float3{0.0f, 2.0f, 0.0f}, float3::zero());

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

				const ray r = cam.spawnRay(u, v);
				color += colorOfRay(r, world);
			}
			color /= float{ samplesPerPixel };

			//Gamma correct
			color = float3{ sqrtf(color.r), sqrtf(color.g), sqrtf(color.b) };

			int ir = int(255.9999f * color.r);
			int ig = int(255.9999f * color.g);
			int ib = int(255.9999f * color.b);

			cout << ir << " " << ig << " " << ib << "\n";
		}
	}

	cout.close();

	system(R"("C:\Program Files\IrfanView\i_view64.exe" .\output.ppm /pos=(400,300))");
}
