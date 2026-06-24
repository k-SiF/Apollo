#include <apollo/entity.h>
#include "entity.h"

namespace apollo {
    Entity::Entity(Mesh *mesh, glm::vec2 position) : m_position(position), m_prevPosition(position) ,m_mesh(mesh) {
    }

    Entity::~Entity() {
    }

    void Entity::update(float deltaTime) { 

    }

    void Entity::fixedUpdate(float deltaTime) {

    }

    void Entity::setTextureSize(Texture* tex, float size) {
        m_tex = tex;
        if (!tex || tex->height() == 0) return;
        float aspect = (float)tex->width() / tex->height();
        m_scale = glm::vec2(aspect, 1.0f) * size;

        if (!m_colSizeSet) {
            m_colSize = glm::abs(m_scale);
        }
    }

    glm::mat4 Entity::getModelMatrix(float alpha) const {
        glm::vec2 pos = m_prevPosition * (1.0f - alpha) + m_position * alpha;
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(pos, 0.0f));// position
        model = glm::rotate(model, glm::radians(m_rotation), glm::vec3(0,0,1)); // rotation (around z, for 2D)
        model = glm::scale(model, glm::vec3(m_scale, 1.0f));          // scale
        return model;
    }

    Collider Entity::getBounds() const {
        return getBoundsAt(m_position);
    }

    Collider Entity::getBoundsAt(glm::vec2 position) const {
        glm::vec2 offset = m_colSize * 0.5f; 
        return { position - offset, position + offset};
    }
}
