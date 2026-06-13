#include "scene.h"

Entity* Scene::addEntity(Mesh *mesh, glm::vec2 position) {
    auto entity = std::make_unique<Entity>(mesh, position); // unique pointer
    Entity* ptr = entity.get();                             // borrow raw pointer
    m_entities.push_back(std::move(entity));                // transfer pointer ownership
    return ptr;
}

void Scene::update(float deltaTime) {
    for (auto& entity : m_entities) {
        entity->update(deltaTime);
    }
}

void Scene::draw(Renderer& renderer) {
    for (auto& entity : m_entities) {
        renderer.draw(*entity);
    }
}
