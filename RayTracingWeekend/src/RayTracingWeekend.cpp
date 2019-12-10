// RayTracingWeekend.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include "float3.h"
#include <fstream>

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
			float r = float(i) / float(nx);
			float g = float(j) / float(ny);
			float b = 0.2f;

			int ir = int(255.99999999f * r);
			int ig = int(255.99999999f * g);
			int ib = int(255.99999999f * b);

			cout << ir << " " << ig << " " << ib << "\n";
		}
	}
	cout.close();

	system(R"("C:\Program Files\IrfanView\i_view64.exe" .\output.ppm /pos=(400,300))");
}
