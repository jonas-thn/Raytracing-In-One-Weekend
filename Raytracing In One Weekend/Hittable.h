#pragma once

#include "Utility.h"
#include "Ray.h"


class HitRecord
{
public:
	point3 p;
	vec3 normal;
	double t;
	bool front_face;

	void SetFaceNormal(const Ray& r, const vec3& outward_normal)
	{
		front_face = dot(r.direction(), outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};

class Hittable
{
public:
	virtual ~Hittable() = default;

	virtual bool Hit(const Ray& r, double ray_tmin, double ray_tmax, HitRecord& rec) const = 0;
};