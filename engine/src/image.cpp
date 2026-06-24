#include <stb_image.h>
#include <apollo/image.h>

namespace apollo {
    DecodedImage DecodedImage::decode(const std::string& filepath) {
        DecodedImage img;
        img.path = filepath;

        stbi_set_flip_vertically_on_load_thread(true);
        img.pixels = stbi_load(filepath.c_str(), &img.width, &img.height, &img.channels, 0);
        img.ok = (img.pixels != nullptr);
        return img;
    }

    void DecodedImage::free() {
        if (pixels) {
            stbi_image_free(pixels);
            pixels = nullptr;
        }
    }
}