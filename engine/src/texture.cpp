#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <apollo/texture.h>
#include <glad/gl.h>
#include <iostream>

namespace apollo {
    Texture::Texture(const std::string& filepath) {
        stbi_set_flip_vertically_on_load(true);

        int channels;
        unsigned char* data = stbi_load(filepath.c_str(), &m_width, &m_height, &channels, 0);
        if (!data) {
            std::cout << "Failed to load texture: " << filepath << std::endl;
            return;
        }

        glGenTextures(1, &m_id);
        glBindTexture(GL_TEXTURE_2D, m_id);

        // wrapping and filtering
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // pick format based on channel (3 = RGB, 4 = RGBA)
        GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data); // GPU has a copy, free CPU pixels.
    }

    Texture::~Texture() {
        glDeleteTextures(1, &m_id);
    }

    Texture::Texture(Texture&& other) noexcept 
        : m_id(other.m_id), m_width(other.m_width), m_height(other.m_height) {
            other.m_id = 0;
    }

    Texture& Texture::operator=(Texture&& other) noexcept {
        if (this != &other) {
            glDeleteTextures(1, &m_id);
            m_id = other.m_id; m_width = other.m_width; m_height = other.m_height;
            other.m_id = 0;
        }
        return *this;
    }

    void Texture::bind(unsigned int slot) const {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, m_id);
    }
}