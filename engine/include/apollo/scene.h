#pragma once
#include <apollo/entity.h>
#include <vector>
#include <memory>

namespace apollo {
    class Renderer;

    class Scene {
        public:
            template<typename T, typename... Args>
            T* addEntity(Args&&... args) {
                auto entity = std::make_unique<T>(std::forward<Args>(args)...);
                T* ptr = entity.get();
                ptr->m_scene = this;
                m_entities.push_back(std::move(entity));
                return ptr;
            }

            Entity* addEntity(Mesh* mesh, glm::vec2 position = glm::vec2(0.0f));
            void update(float dt);
            void fixedUpdate(float dt);
            void draw(Renderer& renderer, float alpha);
            bool wouldCollide(const Collider& box, const Entity* ignore) const;
        
        private:
            struct DrawItem { 
                Entity* entity; 
                int layer, order; 
                float y; 
            };
            std::vector<DrawItem> m_drawList;
            std::vector<std::unique_ptr<Entity>> m_entities;
    };
}