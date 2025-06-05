#pragma once

#include "Utility.h"
#include "Ray.h"
#include "Interval.h"
#include <memory>

class Material;

class HitRecord
{
public:
	point3 p;
	vec3 normal;
	std::shared_ptr<Material> mat;
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

	virtual bool Hit(const Ray& r, Interval ray_t, HitRecord& rec) const = 0;
};