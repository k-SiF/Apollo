#include <apollo/entity.h>

namespace apollo {
    Entity::Entity(Mesh *mesh, glm::vec2 position) : m_position(position), m_mesh(mesh) {
    }

    Entity::~Entity() {
    }

    void Entity::update() {

    }

    glm::mat4 Entity::getModelMatrix() const {
        glm::mat4 model = glm::mat4(1.0f);                                      // identity
        model = glm::translate(model, glm::vec3(m_position, 0.0f));             // position
        model = glm::rotate(model, glm::radians(m_rotation), glm::vec3(0,0,1)); // rotation (around z, for 2D)
        model = glm::scale(model, glm::vec3(m_scale, 1.0f));                    // scale
        return model;
    }
}