#pragma once

#include "Hittable.h"
#include "Utility.h"
#include "Vec3.h"

class Sphere : public Hittable
{
public:
    Sphere(const point3& center, double radius) : center(center), radius(std::fmax(0, radius)) {}

    bool Hit(const Ray& r, Interval ray_t, HitRecord& rec) const override 
    {
        vec3 oc = center - r.origin();
        auto a = r.direction().length_squared();
        auto h = dot(r.direction(), oc);
        auto c = oc.length_squared() - radius * radius;

        auto discriminant = h * h - a * c;
        if (discriminant < 0)
            return false;

        auto sqrtd = std::sqrt(discriminant);

        auto root = (h - sqrtd) / a;
        if (!ray_t.surrounds(root)) 
        {
            root = (h + sqrtd) / a;
            if (!ray_t.surrounds(root))
                return false;
        }

        rec.t = root;
        rec.p = r.at(rec.t);
        vec3 outward_normal = (rec.p - center) / radius;
        rec.SetFaceNormal(r, outward_normal);


        return true;
    }

private:
    point3 center;
    double radius;
};