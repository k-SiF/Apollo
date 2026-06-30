#pragma once
#include <apollo/mesh.h>
#include <apollo/texture.h>
#include <apollo/uvrect.h>
#include <apollo/animation.h>
#include <apollo/audio.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <unordered_map>

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
            
            void addAnimation(const std::string& name, Animation anim, float scale = 1.0f);
            void play(const std::string& name); 
            void updateAnimation(float deltaTime);
            bool hasAnimation() const { return m_currentAnim != nullptr; }

            bool isAlive() const { return m_alive; }
            void destroy() { m_alive = false; }

            glm::vec2 getPosition() const { return m_position; }
            void setPosition(glm::vec2 position) { m_position = position; }
            glm::vec2 getScale() const { return m_scale; }
            void setScale(glm::vec2 scale) { 
                m_scale = scale; 
                if (!m_colSizeSet) m_colSize = glm::abs(scale); 
            }
            glm::vec2 getColSize() const { return m_colSize; }
            void setColSize(glm::vec2 colSize) { m_colSize = colSize; m_colSizeSet = true; }
            float getRotation() const { return m_rotation; }
            void setRotation(float rotation) { 
                if (rotation >= 360.0f || rotation <= -360.0f) rotation = 0.0f;
                m_rotation = rotation;
            }

            bool isCollidable() const { return m_collidable; }
            void setCollidable(bool c) { m_collidable = c; }

            Collider getBounds() const;
            Collider getBoundsAt(glm::vec2 position) const;
            Mesh* getMesh() const { return m_mesh; }
            void setMesh(Mesh* mesh) { m_mesh = mesh; }
            Texture* getTexture() const { return m_tex; }
            void setTexture(Texture* tex) { m_tex = tex; }
            void setTextureSize(Texture* tex, float size);

            // Sprite Sheet rendering helpers
            const UVRect& getUVRect() const { return m_uvRect; }
            void setUVRect(const UVRect& uv) { m_uvRect = uv; }

            glm::vec2 getAnchor() const { return m_anchor; }
            void setAnchor(glm::vec2 anchor) { m_anchor = anchor; }
            void anchorBottom() { m_anchor = glm::vec2(0.0f, 0.5f); }

            Scene* getScene() const { return m_scene; }
            
            glm::vec2 getRenderPosition(float alpha) const {
                return m_prevPosition * (1.0f - alpha) + m_position * alpha;   // lerp
            }
            glm::mat4 getModelMatrix(float alpha) const;
            
        protected:
            glm::vec2 m_position;
            glm::vec2 m_prevPosition;
            glm::vec2 m_scale = glm::vec2(1.0f);
            glm::vec2 m_anchor = glm::vec2(0.0f); // centered anchor
            glm::vec2 m_colSize = glm::vec2(1.0f);
            bool m_colSizeSet = false;       
            bool m_collidable = true;   

            float m_rotation = 0.0f;
            bool m_alive = true;

            Mesh* m_mesh = nullptr;
            Texture* m_tex = nullptr;
            UVRect m_uvRect; 
            Scene* m_scene = nullptr;
            
            struct AnimEntry {
                Animation anim;
                float scale = 1.0f;
            };

            std::unordered_map<std::string, AnimEntry> m_animations;
            Animation* m_currentAnim = nullptr;
            Audio* m_audio = nullptr;
            float m_currentScale = 1.0f;
            std::string m_currentName;

            void sizeToFrame(const Frame& f, float scale);

            friend class Scene;
    }; 
} 