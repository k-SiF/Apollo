#include <apollo/renderer.h>

#include <glad/gl.h>
#include <iostream>

namespace apollo {
    Renderer::Renderer() : m_shader("shaders/triangle.glsl"), m_debugShader("shaders/debug.glsl"), m_textShader("shaders/text.glsl") {
        // Debug shader
        glGenVertexArrays(1, &m_debugVAO);
        glGenBuffers(1, &m_debugVBO);

        glBindVertexArray(m_debugVAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_debugVBO);

        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24, nullptr, GL_DYNAMIC_DRAW); // 8 verts * 3

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glBindVertexArray(0);

        // Text shader
        glGenVertexArrays(1, &m_textVAO);
        glGenBuffers(1, &m_textVBO);

        glBindVertexArray(m_textVAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_textVBO);

        // 6 vertices per glyph, each: vec2 pos + vec2 uv = 4 floats. Dynamic (updated per draw).
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * 6, nullptr, GL_DYNAMIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        glBindVertexArray(0);
    }

    Renderer::~Renderer() {}

    void Renderer::clear(float r, float g, float b, float a) {
        glClearColor(r, g, b, a);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void Renderer::begin(int width, int height, const Camera& camera) {
        float aspect = (float)width / (float)height;
        m_projection = glm::ortho(-aspect, aspect, -1.0f, 1.0f, -1.0f, 1.0f);
        m_view = camera.getViewMatrix();

        m_shader.use();
        m_shader.setMat4("uProjection", m_projection);
        m_shader.setMat4("uView", m_view);
        m_shader.setInt("uTexture", 0);
    }

    void Renderer::draw(Entity &entity, float alpha) {
        m_shader.use();
        m_shader.setMat4("uModel", entity.getModelMatrix(alpha));

        const UVRect& uv = entity.getUVRect();
        m_shader.setVec4("uUVRect", glm::vec4(uv.x, uv.y, uv.w, uv.h));

        if (entity.getTexture()) entity.getTexture()->bind(0);
        entity.getMesh()->draw();

        // --- DEBUG ---
        // static float lastX = 0.0f;
        // float d = entity.getRenderPosition(alpha).x - lastX;
        // lastX = entity.getRenderPosition(alpha).x;
        // if (d > 0.0f && d < 0.1f) std::cout << d << "\n";
        // --------------------------
    }

    void Renderer::drawDebugBox(const Collider& box, glm::vec3 color) {
        glm::vec2 min = box.min;
        glm::vec2 max = box.max;

        float verts[] = {
            // bottom edge
            min.x, min.y, 0.0f,   max.x, min.y, 0.0f,
            // right edge
            max.x, min.y, 0.0f,   max.x, max.y, 0.0f,
            // top edge
            max.x, max.y, 0.0f,   min.x, max.y, 0.0f,
            // left edge
            min.x, max.y, 0.0f,   min.x, min.y, 0.0f,
        };

        // drawDebugBox, after building verts[]:
        m_debugShader.use();
        m_debugShader.setMat4("uProjection", m_projection);
        m_debugShader.setMat4("uView", m_view);
        m_debugShader.setVec3("uColor", color); // green

        glBindVertexArray(m_debugVAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_debugVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verts), verts);  // update with this box

        glDrawArrays(GL_LINES, 0, 8);   // 8 vertices = 4 lines
        
    }

    void Renderer::beginText(int width, int height) {
        // screen-space ortho: (0,0) at TOP-LEFT, (width,height) at bottom-right.
        // note the flipped y (top=0) so text coordinates feel like typical UI.
        m_textProjection = glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);
    }
    
    void Renderer::drawText(const Font& font, const std::string& text,
                            float x, float y, glm::vec3 color) {
        if (!font.valid()) return;
    
        m_textShader.use();
        m_textShader.setMat4("uProjection", m_textProjection);
        m_textShader.setVec3("uColor", color);
        m_textShader.setInt("uText", 0);
    
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, font.atlasTexture());
        glBindVertexArray(m_textVAO);
    
        // pen starts at (x, y). Because stb's glyph yoff is relative to the
        // baseline-ish top, we add the font's pixel height so y is the TOP of the
        // text line (more intuitive than the baseline).
        float penX = x;
        float penY = y + font.lineHeight();
    
        for (char c : text) {
            if (c == '\n') {                       // newline support
                penX = x;
                penY += font.lineHeight();
                continue;
            }
    
            const Glyph& g = font.glyph(c);
    
            // quad corners in pixel space
            float x0 = penX + g.xoff;
            float y0 = penY + g.yoff;
            float x1 = x0 + g.width;
            float y1 = y0 + g.height;
    
            // two triangles (6 verts): pos.xy, uv.xy
            float verts[6][4] = {
                { x0, y0, g.u0, g.v0 },
                { x0, y1, g.u0, g.v1 },
                { x1, y1, g.u1, g.v1 },
    
                { x0, y0, g.u0, g.v0 },
                { x1, y1, g.u1, g.v1 },
                { x1, y0, g.u1, g.v0 },
            };
    
            glBindBuffer(GL_ARRAY_BUFFER, m_textVBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verts), verts);
            glDrawArrays(GL_TRIANGLES, 0, 6);
    
            penX += g.xadvance;   // advance the pen for the next glyph
        }
    
        glBindVertexArray(0);
    }

}
