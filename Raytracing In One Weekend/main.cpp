#include <iostream>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <iostream>
#include <vector>
#include <cstdint>
#include "Vec3.h"

const int width = 256;
const int height = 256;
std::vector<uint8_t> framebuffer(width* height * 4);

using Color = vec3;

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

int main() 
{
    for (int y = 0; y < height; ++y)
    {
        std::cout << "Scanline: " << y << " / " << height << std::endl;

        for (int x = 0; x < width; ++x)
        {
			WriteColor(x, y, Color(x / (double)width, y / (double)height, 0.0));
        }
    }

    Render();

    return 0;
}
