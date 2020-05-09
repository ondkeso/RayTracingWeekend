#pragma once
#include "hittable.h"
#include <vector>

class hittableList : public hittable
{
public:
	hittableList() {}
	hittableList(std::vector<hittable*> hittables)
		: hittables{ hittables } {}

	virtual bool hit(const ray& r, float tMin, float tMax, hitRecord& record) const override;

	std::vector<hittable*> hittables;
};

bool hittableList::hit(const ray& r, float tMin, float tMax, hitRecord& record) const
{
	hitRecord closestHitRecord;
	closestHitRecord.t = tMax;
	bool hitAnything = false;

	for (const hittable* item : hittables)
	{
		if (item->hit(r, tMin, closestHitRecord.t, closestHitRecord))
		{
			hitAnything = true;
		}
	}

	record = closestHitRecord;
	return hitAnything;
}
