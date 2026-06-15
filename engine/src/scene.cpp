#include <apollo/scene.h>
#include <apollo/renderer.h> 
#include <apollo/chrono.h>

namespace apollo {
    Entity* Scene::addEntity(Mesh *mesh, glm::vec2 position) {
        auto entity = std::make_unique<Entity>(mesh, position); // unique pointer
        Entity* ptr = entity.get();                             // borrow raw pointer
        m_entities.push_back(std::move(entity));                // transfer pointer ownership
        return ptr;
    }

    void Scene::update() {
        for (auto& entity : m_entities) {
            entity->update();
        }

        std::erase_if(m_entities, [](const auto& e) { return !e->isAlive(); });
    }

    void Scene::draw(Renderer& renderer) {
        for (auto& entity : m_entities) {
            renderer.draw(*entity);
        }
    }
}