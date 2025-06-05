#pragma once

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "Hittable.h"
#include "Vec3.h"
#include "Ray.h"
#include <vector>
#include "Material.h"

using Color = vec3;

class Camera 
{
public:
    double aspect_ratio = 16.0 / 9.0;
    int width = 400;
    int samples_per_pixel = 10;
    int max_depth = 10;

    double vfov = 90;
    point3 lookfrom = point3(0, 0, 0);   
    point3 lookat = point3(0, 0, -1); 
    vec3   vup = vec3(0, 1, 0);     

    double defocus_angle = 0;  
    double focus_dist = 10;

    void render(const Hittable& world) 
    {
        initialize();

        for (int y = 0; y < height; ++y)
        {
            std::cout << "Scanline: " << y + 1 << " / " << height << std::endl;

            for (int x = 0; x < width; ++x)
            {
                Color pixel_color(0, 0, 0);
                for (int sample = 0; sample < samples_per_pixel; sample++)
                {
                    Ray r = get_ray(x, y);
                    pixel_color += ray_color(r, max_depth, world);
                }
                WriteColor(x, y, pixel_samples_scale * pixel_color);
            }
        }
        if (stbi_write_png("output.png", width, height, 3, framebuffer.data(), width * 3))
        {
            std::cout << "PNG written successfully.\n";
        }
        else
        {
            std::cerr << "Failed to write PNG.\n";
        }

    }

private:

    int height;
    point3 center;
    point3 pixel00_loc;
    vec3 pixel_delta_u;
    vec3 pixel_delta_v;
    double pixel_samples_scale;
    vec3   u, v, w;
    vec3   defocus_disk_u;       
    vec3   defocus_disk_v;

    std::vector<uint8_t> framebuffer;

    void initialize() 
    {
        height = int(width / aspect_ratio);
        height = (height < 1) ? 1 : height;

        pixel_samples_scale = 1.0 / samples_per_pixel;

        center = lookfrom;

        auto theta = degrees_to_radians(vfov);
        auto h = std::tan(theta / 2);
        auto viewport_height = 2 * h * focus_dist;
        double viewport_width = viewport_height * aspect_ratio;

        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        vec3 viewport_u = viewport_width * u;   
        vec3 viewport_v = viewport_height * -v;  

        pixel_delta_u = viewport_u / width;
        pixel_delta_v = viewport_v / height;

        auto viewport_upper_left = center - (focus_dist * w) - viewport_u / 2 - viewport_v / 2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

        auto defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle / 2));
        defocus_disk_u = u * defocus_radius;
        defocus_disk_v = v * defocus_radius;

        framebuffer = std::vector<uint8_t>(width * height * 3);
    }

    Ray get_ray(int i, int j) const 
    {
        auto offset = sample_square();
        auto pixel_sample = pixel00_loc
            + ((i + offset.x()) * pixel_delta_u)
            + ((j + offset.y()) * pixel_delta_v);

        auto ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
        auto ray_direction = pixel_sample - ray_origin;

        return Ray(ray_origin, ray_direction);
    }

    vec3 sample_square() const 
    {
        return vec3(random_double() - 0.5, random_double() - 0.5, 0);
    }

    point3 defocus_disk_sample() const 
    {
        auto p = random_in_unit_disk();
        return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
    }

    Color ray_color(const Ray& r, int depth, const Hittable& world) const 
    {
        if (depth <= 0)
            return Color(0, 0, 0);


        HitRecord rec;
        if (world.Hit(r, Interval(0.001, infinity), rec))
        {
            Ray scattered;
            Color attenuation;
            if (rec.mat->scatter(r, rec, attenuation, scattered))
            {
                return attenuation * ray_color(scattered, depth - 1, world);
            }

            return Color(0, 0, 0);
        }

        vec3 unit_direction = unit_vector(r.direction());
        double a = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
    }

    void WriteColor(const int x, const int y, const Color& pixel_color)
    {
        int idx = (y * width + x) * 3;

        auto r = pixel_color.x();
        auto g = pixel_color.y();
        auto b = pixel_color.z();

        static const Interval intensity(0.000, 0.999);

        framebuffer[idx + 0] = uint8_t(255.99 * intensity.clamp(r));
        framebuffer[idx + 1] = uint8_t(255.99 * intensity.clamp(g));
        framebuffer[idx + 2] = uint8_t(255.99 * intensity.clamp(b));
    }
};