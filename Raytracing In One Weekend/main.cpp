#include <iostream>

#include <cstdint>
#include <vector>
#include "Vec3.h"
#include "Ray.h"

#include "Utility.h"
#include "Hittable.h"
#include "HittableList.h"
#include "Sphere.h"
#include "Camera.h"

using Color = vec3;

double HitSphere(const point3& center, double radius, const Ray& r)
{
	vec3 oc = center - r.origin();
    double a = r.direction().length_squared();
    double h = dot(r.direction(), oc);
	double c = oc.length_squared() - radius * radius;
	double discriminant = h*h - a * c;

    if (discriminant < 0)
    {
        return -1.0;
    }
    else
    {
        return (h - std::sqrt(discriminant)) / a;
    }
}

int main() 
{
    HittableList world;

    auto R = std::cos(pi / 4);

    auto material_left = make_shared<Lambertian>(Color(0, 0, 1));
    auto material_right = make_shared<Lambertian>(Color(1, 0, 0));

    world.add(make_shared<Sphere>(point3(-R, 0, -1), R, material_left));
    world.add(make_shared<Sphere>(point3(R, 0, -1), R, material_right));


    Camera cam;
    cam.aspect_ratio = 16.0 / 9.0;
    cam.width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 10;
    cam.vfov = 90;

    cam.render(world);

    return 0;
}
