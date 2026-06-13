#include "entity.h"

Entity::Entity(Mesh *mesh, glm::vec2 position) {
    m_position = position;
    m_mesh = mesh;
}

Entity::~Entity() {
}

void Entity::update(float deltaTime) {

}