#pragma once
#include "hittable.h"

class hittableList : public hittable
{
public:
	hittableList() {}
	hittableList(hittable** list, size_t numHittables)
		: list{ list }
		, numHittables{ numHittables } {}

	virtual bool hit(const ray& r, float tMin, float tMax, hitRecord& record) const override;

	hittable** list;
	size_t numHittables;
};

bool hittableList::hit(const ray& r, float tMin, float tMax, hitRecord& record) const
{
	hitRecord closestHitRecord;
	closestHitRecord.t = tMax;
	bool hitAnything = false;

	for (size_t i = 0; i < numHittables; ++i)
	{
		if (list[i]->hit(r, tMin, closestHitRecord.t, closestHitRecord))
		{
			hitAnything = true;
		}
	}

	record = closestHitRecord;
	return hitAnything;
}
