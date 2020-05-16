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
#include <memory>

float3 colorOfRay(const ray& r, const hittable& hittable, int recursionsLeft)
{
	hitRecord record;
	if (hittable.hit(r, 0.001f, std::numeric_limits<float>::max(), record))
	{
		ray transmitted;
		float3 attenuation;
		if (recursionsLeft-- > 0 && record.material->transmit(r, record, &attenuation, &transmitted))
		{
			return attenuation * colorOfRay(transmitted, hittable, recursionsLeft);
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

hittableList randomSphereScene()
{
	std::vector<hittable*> hittables;

	hittables.emplace_back(new sphere{ float3{ 0.0f, -1000.0f, 0.0f }, 1000, new lambertian{ float3::gray() } });

	static constexpr float smallRadius = 0.2f;
	static constexpr float bigRadius = 1.0f;
	static const dielectric* glassMaterial = new dielectric{ 1.5f };

	for (int x = -11; x < 12; ++x)
	{
		for (int z = -11; z++ < 12; z++)
		{
			float3 position{x + 0.9f * random01(), smallRadius, z + 1.3f * random01()};
			if (((position - float3{ 4.0f, 0.2f, 0.0f }).squaredLength() > 0.81f))
			{
				const float materialChoice = random01();
				if (materialChoice < 0.75f)
				{
					//diffuse
					const auto albedo = float3{ random01(), random01(), random01()};
					hittables.emplace_back(new sphere{ position, smallRadius, new lambertian{ albedo } });
				}
				else if (materialChoice < 0.9f)
				{
					//metal
					const auto albedo = float3{ random(0.5, 1.0f), random(0.5, 1.0f), random(0.5, 1.0f)};
					const auto roughness = random(0.0f, 0.5f);
					hittables.emplace_back(new sphere{ position, smallRadius, new metal{ albedo, roughness }});
				}
				else
				{
					//glass
					hittables.emplace_back(new sphere{ position, smallRadius, glassMaterial});
				}
			}
		}
	}

	hittables.emplace_back(new sphere{ float3{ 0.0f, 1.0f, 0.0f }, bigRadius, glassMaterial });
	hittables.emplace_back(new sphere{ float3{ -4.0f, 1.0f, 0.0f }, bigRadius, new lambertian { float3{ 0.4f, 0.2f, 0.1f } } });
	hittables.emplace_back(new sphere{ float3{ 4.0f, 1.0f, 0.0f }, bigRadius, new metal { float3{ 0.7f, 0.6f, 0.5f }, 0.0f } });

	return hittableList{hittables};
}

int main()
{
	// Quality settings
	constexpr int maxRecursion = 25;
	constexpr float aspectRatio = 16.0f / 9.0f;
	constexpr int imageHeight = 1440;
	constexpr int imageWidth = (int)(imageHeight * aspectRatio);
	constexpr int samplesPerPixel = 90;

	// Camera settings
	constexpr float3 viewUp{ 0.0f, 1.0f, 0.0f };
	constexpr float3 origin{ 13.0f, 2.0f, -3.0f };
	constexpr float3 lookAt{ 0.0f, 0.0f, 0.0f };
	constexpr float fov = 20.0f;
	constexpr float aperatureRadius = 0.05f;
	const float focusDistance = 10.0f;

	hittableList sphereScene = randomSphereScene();

	//sphereScene.emplace_back(new sphere{ float3{0.0f, -100.5f, 1.0f}, 100, new lambertian{float3{0.8f, 0.8f, 0.0f }} });
	//sphereScene.emplace_back(new sphere{ float3{-1.5002f, 0.0f, 1.7f}, 0.5f, new lambertian{float3{0.1f, 0.0f, 0.5f }} });
	//sphereScene.emplace_back(new sphere{ float3{-0.5001f, 0.0f, 1.7f}, 0.5f, new dielectric{1.5f} });
	//sphereScene.emplace_back(new sphere{ float3{0.5001f, 0.0f, 1.7f}, 0.5f, new metal{float3{0.8f, 0.6f, 0.2f }, 0.1f} });
	//sphereScene.emplace_back(new sphere{ float3{1.5002f, 0.0f, 1.7f}, -0.5f, new dielectric{1.5f} });

	const hittableList world{ sphereScene };
	const camera cam(origin, lookAt, viewUp, fov, aspectRatio, aperatureRadius, focusDistance);

	std::ofstream cout("output.ppm");
	cout << "P3\n" << imageWidth << " " << imageHeight << "\n255\n";

	for (int j = imageHeight - 1; j >= 0; j--)
	{
		for (int i = 0; i < imageWidth; i++)
		{
			float3 color{0.0f, 0.0f, 0.0f};

			for (int s = 0; s < samplesPerPixel; ++s)
			{
				const float u = (i + random01()) / float{ imageWidth };
				const float v = (j + random01()) / float{ imageHeight };

				const ray r = cam.spawnRay(u, v);
				color += colorOfRay(r, world, maxRecursion);
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
