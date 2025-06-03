#pragma once

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "Hittable.h"
#include "Vec3.h"
#include "Ray.h"
#include <vector>

using Color = vec3;

class Camera 
{
public:
    double aspect_ratio = 16.0 / 9.0;
    int width = 400;

    void render(const Hittable& world) 
    {
        initialize();

        for (int y = 0; y < height; ++y)
        {
            std::cout << "Scanline: " << y + 1 << " / " << height << std::endl;

            for (int x = 0; x < width; ++x)
            {
                vec3 pixel_center = pixel00_loc + (x * pixel_delta_u) + (y * pixel_delta_v);
                vec3 ray_direction = pixel_center - center;
                Ray r(center, ray_direction);

                Color pixel_color = ray_color(r, world);

                WriteColor(x, y, pixel_color);
            }
        }
    }

private:

    int height;
    point3 center;
    point3 pixel00_loc;
    vec3 pixel_delta_u;
    vec3 pixel_delta_v;

    std::vector<uint8_t> framebuffer;

    void initialize() 
    {
        height = int(width / aspect_ratio);
        height = (height < 1) ? 1 : height;
        center = point3(0, 0, 0);

        double focal_length = 1.0;
        double viewport_height = 2.0;
        double viewport_width = viewport_height * aspect_ratio;
        vec3 camera_center = point3(0, 0, 0);

        vec3 viewport_u = vec3(viewport_width, 0, 0);
        vec3 viewport_v = vec3(0, -viewport_height, 0);

        vec3 pixel_delta_u = viewport_u / width;
        vec3 pixel_delta_v = viewport_v / height;

        vec3 viewport_upper_left = camera_center - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
        vec3 pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

        framebuffer = std::vector<uint8_t>(width * height * 3);
    }

    Color ray_color(const Ray& r, const Hittable& world) const 
    {
        HitRecord rec;
        if (world.Hit(r, Interval(0, infinity), rec))
        {
            return 0.5 * (rec.normal + Color(1, 1, 1));
        }

        vec3 unit_direction = unit_vector(r.direction());
        double a = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
    }

    void WriteColor(const int x, const int y, const Color& pixel_color)
    {
        int idx = (y * width + x) * 3;
        framebuffer[idx + 0] = uint8_t(pixel_color.x() * 255.99);
        framebuffer[idx + 1] = uint8_t(pixel_color.y() * 255.99);
        framebuffer[idx + 2] = uint8_t(pixel_color.z() * 255.99);
    }

    void Render()
    {
        if (stbi_write_png("output.png", width, height, 3, framebuffer.data(), width * 3))
        {
            std::cout << "PNG written successfully.\n";
        }
        else
        {
            std::cerr << "Failed to write PNG.\n";
        }
    }
};