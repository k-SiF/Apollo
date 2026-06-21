#include <apollo/renderer.h>

#include <glad/gl.h>
#include <iostream>

namespace apollo {
    Renderer::Renderer() : m_shader("shaders/triangle.glsl"), m_debugShader("shaders/debug.glsl") {
        glGenVertexArrays(1, &m_debugVAO);
        glGenBuffers(1, &m_debugVBO);

        glBindVertexArray(m_debugVAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_debugVBO);

        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24, nullptr, GL_DYNAMIC_DRAW); // 8 verts * 3

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

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
}
