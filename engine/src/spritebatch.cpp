#include <apollo/spritebatch.h>
#include <glad/gl.h>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

namespace apollo {
    SpriteBatch::SpriteBatch() : m_shader("shaders/batch.glsl") {
        m_vertices.reserve(MAX_VERTICES);

        glGenVertexArrays(1, &m_vao);
        glGenBuffers(1, &m_vbo);
        glGenBuffers(1, &m_ebo);

        glBindVertexArray(m_vao);

        // Dynamic vertex buffer
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, MAX_VERTICES * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

        // Indices remain static, each quad is two triangles
        // Precompute once for all MAX_SPRITES quads
        std::vector<unsigned int> indices(MAX_INDICES);
        unsigned int offset = 0;
        for (int i = 0; i < MAX_INDICES; i += 6) {
            indices[i + 0] = offset + 0;
            indices[i + 1] = offset + 1;
            indices[i + 2] = offset + 2;
            indices[i + 3] = offset + 2;
            indices[i + 4] = offset + 3;
            indices[i + 5] = offset + 0;
            offset += 4;
        }
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, MAX_INDICES * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        // Vertext layout
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));

        glBindVertexArray(0);
    }

    SpriteBatch::~SpriteBatch() {
        glDeleteVertexArrays(1, &m_vao);
        glDeleteBuffers(1, &m_vbo);
        glDeleteBuffers(1, &m_ebo);
    }

    void SpriteBatch::begin(const glm::mat4& viewProjection) {
        m_viewProjection = viewProjection;
        m_currentTexture = nullptr;
        m_vertices.clear();
        m_drawCalls = 0;
        m_spritesThisFrame = 0;
    }

    void SpriteBatch::draw(Texture* texture, glm::vec2 center, glm::vec2 size, 
                           const UVRect& uv, float rotationDeg, glm::vec4 color) {
        if (!texture) return;

        // Texture changed, flush
        if ((m_currentTexture && texture != m_currentTexture) || m_vertices.size() + 4 > (size_t)MAX_VERTICES) {
            flush();
        }
        m_currentTexture = texture;

        glm::vec2 half = size * 0.5f;
        glm::vec2 corners[4] = {
            { -half.x, -half.y},    // bottom left
            {  half.x, -half.y},    // bottom right
            {  half.x,  half.y},    // top right
            { -half.x,  half.y},    // top left
        };

        // Rotation, around center of sprite
        if (rotationDeg != 0.0f) {
            float r = glm::radians(rotationDeg);
            float c = std::cos(r), s = std::sin(r);
            for (auto& p : corners) {
                p = glm::vec2(p.x * c - p.y * s, p.x * s + p.y * c);
            }
        }

        // Translate to world position
        for (auto& p : corners) p += center;

        // UV corners
        glm::vec2 uvs[4] = {
            { uv.x, uv.y },
            { uv.x + uv.w, uv.y },
            { uv.x + uv.w, uv.y + uv.h },
            { uv.x, uv.y + uv.h}
        };

        for (int i = 0; i < 4; i++) {
            m_vertices.push_back({corners[i], uvs[i], color});
        }
        m_spritesThisFrame++;
    }

    void SpriteBatch::flush() {
        if (m_vertices.empty() || !m_currentTexture) return;

        m_shader.use();
        m_shader.setMat4("uViewProjection", m_viewProjection);
        m_shader.setInt("uTexture", 0);

        glActiveTexture(GL_TEXTURE0);
        m_currentTexture->bind(0);

        glBindVertexArray(m_vao);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, m_vertices.size() * sizeof(Vertex), m_vertices.data());

        int quadCount = (int)m_vertices.size() / 4;
        glDrawElements(GL_TRIANGLES, quadCount * 6, GL_UNSIGNED_INT, 0);
        m_drawCalls++;

        m_vertices.clear();
    }

    void SpriteBatch::end() {
        flush(); 
        glBindVertexArray(0);
    }
}