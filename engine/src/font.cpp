#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>

#include <apollo/font.h>
#include <glad/gl.h>
#include <fstream>
#include <vector>
#include <iostream>

namespace apollo {
    Font::Font(const std::string& path, float pixelHeight) : m_pixelHeight(pixelHeight) {
        // read .ttf into memory
        std::ifstream file(path, std::ios::binary | std::ios::ate);
        if (!file) {
            std::cout << "Failed to open font: " << path << std::endl;
            return;
        }

        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);
        std::vector<unsigned char> ttf(size);
        if (!file.read(reinterpret_cast<char*>(ttf.data()), size)) {
            std::cout << "Failed to read font: " << path << std::endl;
            return;
        }

        // bake ASCII glyphs into single-channel atlas bitmap
        m_atlasWidth = 512;
        m_atlasHeight = 512;
        std::vector<unsigned char> bitmap(m_atlasWidth * m_atlasHeight);
        std::vector<stbtt_bakedchar> cdata(CHAR_COUNT);

        int result = stbtt_BakeFontBitmap(
            ttf.data(), 0,
            pixelHeight,
            bitmap.data(), m_atlasWidth, m_atlasHeight,
            FIRST_CHAR, CHAR_COUNT,
            cdata.data());
        
        if (result <= 0) {
            std::cout << "Font atlas too small for " << path << " at " << pixelHeight << "px" << std::endl;
        }

        // convert baked char data into Glyph structs
        for (int i = 0; i < CHAR_COUNT; i++) {
            const stbtt_bakedchar& b = cdata[i];
            Glyph& g = m_glyphs[i];
            g.u0 = (float)b.x0 / m_atlasWidth;
            g.v0 = (float)b.y0 / m_atlasHeight;
            g.u1 = (float)b.x1 / m_atlasWidth;
            g.v1 = (float)b.y1 / m_atlasHeight;
            g.width = (float)(b.x1 - b.x0);
            g.height = (float)(b.y1 - b.y0);
            g.xoff = b.xoff;
            g.yoff = b.yoff;
            g.xadvance = b.xadvance;
        }

        // upload atlas to GPU, single channel (alpha)
        glGenTextures(1, &m_textureID);
        glBindTexture(GL_TEXTURE_2D, m_textureID);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 
                     m_atlasWidth, m_atlasHeight, 0,
                     GL_RED, GL_UNSIGNED_BYTE, bitmap.data());
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 4); // restore to default
    }

    Font::~Font() {
        if (m_textureID) glDeleteTextures(1, &m_textureID);
    }

    const Glyph& Font::glyph(char c) const {
        int i = (int)c - FIRST_CHAR;
        if (i < 0 || i >= CHAR_COUNT) i = 0; // out of range
        return m_glyphs[i];
    }
}