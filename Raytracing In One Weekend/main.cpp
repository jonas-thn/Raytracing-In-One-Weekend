#include <iostream>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <iostream>
#include <vector>
#include <cstdint>

const int width = 256;
const int height = 256;
std::vector<uint8_t> framebuffer(width* height * 4);

void Render()
{
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            int idx = (y * width + x) * 4;
            framebuffer[idx + 0] = 255;
            framebuffer[idx + 1] = 0;
            framebuffer[idx + 2] = 0;
            framebuffer[idx + 3] = (uint8_t)(255.0f * (float)x / width);
        }
    }
}

void ProduceImage()
{
    if (stbi_write_png("output.png", width, height, 4, framebuffer.data(), width * 4))
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
    Render();
    ProduceImage();

    return 0;
}
