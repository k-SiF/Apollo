#pragma once
#include <apollo/mesh.h>
#include <apollo/texture.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 

namespace apollo {
    class Scene;

    struct Collider {
        glm::vec2 min, max;
        bool collision;
        bool intersect(const Collider& other) const {
            return min.x < other.max.x && max.x > other.min.x &&
            min.y < other.max.y && max.y > other.min.y;
        }
    };

    class Entity {
        public:
            Entity(Mesh* mesh, glm::vec2 position = glm::vec2(0.0f));
            virtual ~Entity();
            virtual void update(float deltaTime);
            virtual void fixedUpdate(float deltaTime);
            virtual void onCollision(Entity& other) {}

            bool isAlive() const { return m_alive; }
            void destroy() { m_alive = false; }

            glm::vec2 getPosition() const { return m_position; }
            void setPosition(glm::vec2 position) { m_position = position; }
            glm::vec2 getScale() const { return m_scale; }
            void setScale(glm::vec2 scale) { m_scale = scale; }
            glm::vec2 getColScale() const { return m_colScale; }
            void setColScale(glm::vec2 colScale) { m_colScale = colScale; }
            float getRotation() const { return m_rotation; }
            void setRotation(float rotation) { 
                if (rotation >= 360.0f || rotation <= -360.0f) rotation = 0.0f;
                m_rotation = rotation;
            }
            Collider getBounds() const;
            Collider getBoundsAt(glm::vec2 position) const;
            Mesh* getMesh() const { return m_mesh; }
            void setMesh(Mesh* mesh) { m_mesh = mesh; }
            Texture* getTexture() const { return m_tex; }
            void setTexture(Texture* tex) { m_tex = tex; }
            void setTextureSize(Texture* tex, float size);

            Scene* getScene() const { return m_scene; }
            
            glm::vec2 getRenderPosition(float alpha) const {
                return m_prevPosition * (1.0f - alpha) + m_position * alpha;   // lerp
            }
            glm::mat4 getModelMatrix(float alpha) const;
            
        protected:
            glm::vec2 m_position;
            glm::vec2 m_prevPosition;
            glm::vec2 m_scale = glm::vec2(1.0f);
            glm::vec2 m_colScale = glm::vec2(1.0f);

            float m_rotation = 0.0f;
            bool m_alive = true;

            Mesh* m_mesh = nullptr;
            Texture* m_tex = nullptr;
            Scene* m_scene = nullptr;
            friend class Scene;
    }; 
} 