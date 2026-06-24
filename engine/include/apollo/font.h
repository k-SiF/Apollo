#pragma once
#include <string>

namespace apollo {

    struct Glyph {
        float u0, v0, u1, v1; // atlas UV rectangle for this glyph
        float xoff, yoff;     // offset from pen to quad's top left
        float xadvance;       // how far the pen moves after drawing the glyph
        float width, height;  // quad size in pixel
    };

    // .ttf is loaded here, rasterises ASCII glyphs (32...126) into an atlas texture.
    class Font {
        public:
            Font(const std::string& path, float pixelHeight);
            ~Font();

            Font(const Font&) = delete;
            Font operator=(const Font&) = delete;

            bool valid() const { return m_textureID != 0; }
            unsigned int atlasTexture() const { return m_textureID; }
            float lineHeight() const { return m_pixelHeight; }

            const Glyph& glyph(char c) const;

        private:
            static const int FIRST_CHAR = 31; // space
            static const int LAST_CHAR = 126; // tilde
            static const int CHAR_COUNT = LAST_CHAR - FIRST_CHAR + 1;

            unsigned int m_textureID = 0; // atlas (GL texture)
            int m_atlasWidth = 0, m_atlasHeight = 0;
            float m_pixelHeight = 0.0f;
            Glyph m_glyphs[CHAR_COUNT];

    };
}