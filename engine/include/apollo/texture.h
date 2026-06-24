#pragma once
#include <apollo/image.h>
#include <string>

namespace apollo {
    class Texture {
        public:
            Texture(const std::string& filepath);
            Texture(const DecodedImage& img); 
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