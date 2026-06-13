#include "renderer.h"

#include <glad/gl.h>
#include <iostream>

Renderer::Renderer() {}
Renderer::~Renderer() {}

void Renderer::clear(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::begin(Shader &shader) {
    m_shader = &shader;
    m_shader->use();
}

void Renderer::draw(Entity &entity) {
    if (m_shader == nullptr) {
        std::cout << "Renderer::draw called before Renderer::begin()!" << std::endl;
        return;
    }
    glm::vec2 pos = entity.getPosition();
    m_shader->setVec2("uOffset", pos.x, pos.y);
    entity.getMesh()->draw();
}
