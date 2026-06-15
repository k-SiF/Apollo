#include "renderer.h"

#include <glad/gl.h>
#include <iostream>

namespace apollo {
    Renderer::Renderer() {}
    Renderer::~Renderer() {}

    void Renderer::clear(float r, float g, float b, float a) {
        glClearColor(r, g, b, a);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void Renderer::begin(Shader &shader, int width, int height) {
        m_shader = &shader;
        m_shader->use();

        float aspect = (float)width / (float)height;
        m_projection = glm::ortho(-aspect, aspect, -1.0f, 1.0f, -1.0f, 1.0f);
        m_shader->setMat4("uProjection", m_projection);
        m_shader->setInt("uTexture", 0);
    }

    void Renderer::draw(Entity &entity) {
        if (m_shader == nullptr) {
            std::cout << "Renderer::draw called before Renderer::begin()!" << std::endl;
            return;
        }
        glm::vec2 pos = entity.getPosition();
        m_shader->setMat4("uModel", entity.getModelMatrix());
        if (entity.getTexture()) {
            entity.getTexture()->bind(0);
        }
        entity.getMesh()->draw();
    }
}
