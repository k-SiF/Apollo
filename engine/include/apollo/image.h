#pragma once
#include <string>

namespace apollo {
    // Raw decoded pixels from stb_image.
    struct DecodedImage {
        std::string path;
        unsigned char* pixels = nullptr; 
        int width = 0;
        int height = 0;
        int channels = 0;
        bool ok = false;

        static DecodedImage decode(const std::string& filepath);

        void free();
    };
}