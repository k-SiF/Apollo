#include <apollo/scene.h>
#include <apollo/renderer.h>
#include <apollo/chrono.h>
#include <algorithm>
#include <iostream>

namespace apollo {
    Entity* Scene::addEntity(Mesh *mesh, glm::vec2 position) {
        auto entity = std::make_unique<Entity>(mesh, position); // unique pointer
        Entity* ptr = entity.get();                             // borrow raw pointer
        ptr->m_scene = this;
        m_entities.push_back(std::move(entity));                // transfer pointer ownership
        return ptr;
    }

    void Scene::update(float dt) {
        for (auto& entity : m_entities) {
            entity->update(dt);
        }
    }

    void Scene::fixedUpdate(float dt) {
        for (auto& entity : m_entities) {
            entity->m_prevPosition = entity->m_position;
            entity->fixedUpdate(dt);
        }

        for (size_t i = 0; i < m_entities.size(); i++) {
            if (!m_entities[i]->isCollidable()) continue;
            for (size_t j = i + 1; j < m_entities.size(); j++) {
                Entity& a = *m_entities[i];
                Entity& b = *m_entities[j];
                Collider aCol = a.getBounds();
                Collider bCol = b.getBounds();

                if (aCol.intersect(bCol)) {
                    a.onCollision(b);
                    b.onCollision(a);
                }
            }
        }

        std::erase_if(m_entities, [](const auto& e) { return !e->isAlive(); });
    }

    void Scene::draw(Renderer& renderer, float alpha) {
        // Build the frame's draw list. y is only meaningful for ySort entities;
        // others get 0 so stable_sort keeps their insertion order within a layer.
        m_drawList.clear();
        for (auto& e : m_entities) {
            if (!e->getTexture()) continue;
            float y = e->ySort() ? e->getRenderPosition(alpha).y : 0.0f;
            m_drawList.push_back({ e.get(), e->getSortLayer(), e->getOrderInLayer(), y });
        }

        std::stable_sort(m_drawList.begin(), m_drawList.end(),
            [](const DrawItem& a, const DrawItem& b) {
                if (a.layer != b.layer) return a.layer < b.layer;
                if (a.order != b.order) return a.order < b.order;
                return a.y > b.y; // larger y = further back (drawn first); smaller y ends up on top
            });

        renderer.beginBatch();
        for (auto& d : m_drawList) {
            renderer.batchDraw(*d.entity, alpha);
        }
        renderer.endBatch();

        if (renderer.m_debugDraw) {
            for (auto& e : m_entities) {
                renderer.drawDebugBox(e->getBounds());
            }
        }
    }

    bool Scene::wouldCollide(const Collider& box, const Entity* ignore) const {
        for (auto& e : m_entities) {
            if (e.get() == ignore || !e->isCollidable()) continue;
            if (box.intersect(e->getBounds())) return true;
        }
        return false;
    }
}