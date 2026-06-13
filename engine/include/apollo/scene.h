#pragma once
#include <apollo/entity.h>
#include <apollo/renderer.h>
#include <vector>
#include <memory>

class Scene {
    public:
        Entity* addEntity(Mesh* mesh, glm::vec2 position = glm::vec2(0.0f));
        void update(float deltaTime);
        void draw(Renderer& renderer);
    
    private:
        std::vector<std::unique_ptr<Entity>> m_entities;
};