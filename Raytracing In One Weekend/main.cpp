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
    world.add(make_shared<Sphere>(point3(0, 0, -1), 0.5));
    world.add(make_shared<Sphere>(point3(0, -100.5, -1), 100));

    Camera cam;
    cam.aspect_ratio = 16.0 / 9.0;
    cam.width = 400;
    cam.samples_per_pixel = 100;

    cam.render(world);

    return 0;
}
