#pragma once
#include <apollo/image.h>
#include <string>

namespace apollo {

    enum class TextureFilter {
        Linear,
        Nearest
    };

    class Texture {
        public:
            Texture(const std::string& filepath, TextureFilter filter = TextureFilter::Linear);
            Texture(const DecodedImage& img, TextureFilter filter = TextureFilter::Linear); 
            ~Texture();

            Texture(const Texture&) = delete;               // no copying
            Texture& operator=(const Texture&) = delete;
            Texture(Texture&& other) noexcept;              // movable
            Texture& operator=(Texture&& other) noexcept;

            void bind(unsigned int slot = 0) const;
            unsigned int id() const { return m_id; }
            int width() const { return m_width; }
            int height() const { return m_height; }
        
        private:
            unsigned int m_id = 0;
            int m_width = 0;
            int m_height = 0;
    };
}