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

int main()
{
	constexpr float sizeFactor = 1.3f;
	constexpr int maxRecursion = 20;
	constexpr float aspectRatio = 16.0f / 9.0f;
	constexpr int imageHeight = (int)(360 * sizeFactor);
	constexpr int imageWidth = (int)(imageHeight * aspectRatio);
	constexpr int samplesPerPixel = 15;

	std::vector<hittable*> sphereScene;
	//sphereScene.emplace_back(sphere{ float3{0.0f, -100.5f, 1.7f}, 100, new lambertian{float3{0.8f, 0.8f, 0.0f }} });
	//sphereScene.emplace_back(sphere{ float3{0.0f, 0.0f, 1.7f}, 0.5f, new lambertian{float3{0.1f, 0.0f, 0.5f }} });
	//sphereScene.emplace_back(sphere{ float3{-1.0f, 0.0f, 1.2f}, 0.5f, new dielectric{1.3f} });
	//sphereScene.emplace_back(sphere{ float3{1.2f, 0.0f, 1.2f}, 0.5f, new dielectric{0.7f} });

	//float x = -4.5f;
	//for (int i = 0; i < 10; ++i)
	//{
	//	sphereScene.emplace_back(new sphere{ float3{x++, 0.9f, -2.2f}, 0.5f, new lambertian{float3{0.01f, 0.01f, 0.01f }} });
	//}
	//sphereScene.emplace_back(new sphere{ float3{0.0f, -100.5f, 1.0f}, 100, new lambertian{float3{0.8f, 0.8f, 0.0f }} });
	//
	//sphereScene.emplace_back(new sphere{ float3{-1.6f, 0.0f, 1.7f}, 0.5f, new lambertian{float3{0.1f, 0.0f, 0.5f }} });
	//sphereScene.emplace_back(new sphere{ float3{-0.5001f, 0.0f, 1.7f}, -0.5f, new dielectric{1.5f} });
	//sphereScene.emplace_back(new sphere{ float3{0.5001f, 0.0f, 1.7f}, 0.5f, new metal{float3{0.8f, 0.6f, 0.2f }, 0.1f} });
	//sphereScene.emplace_back(new sphere{ float3{1.6f, 0.0f, 1.7f}, 0.5f, new dielectric{1.5f} });

	const float r = 0.5f;
	const float d = 1.0f;
	sphereScene.push_back(new sphere{ float3{0.0f, 0.0f, d}, r, new lambertian{float3::blue()} });

	const hittableList world{ sphereScene };
	const float fovToTangentSphere = 2.0f * asinf(r / d) * RAD_TO_DEG;
	const camera cam(fovToTangentSphere, aspectRatio);

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
