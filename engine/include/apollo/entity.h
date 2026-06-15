#pragma once
#include <apollo/mesh.h>
#include <apollo/texture.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 

namespace apollo {
    class Entity {
        public:
            Entity(Mesh* mesh, glm::vec2 position = glm::vec2(0.0f));
            virtual ~Entity();
            virtual void update();

            glm::vec2 getPosition() const { return m_position; }
            void setPosition(glm::vec2 position) { m_position = position; }
            glm::vec2 getScale() const { return m_scale; }
            void setScale(glm::vec2 scale) { m_scale = scale; }
            float getRotation() const { return m_rotation; }
            void setRotation(float rotation) { 
                if (rotation >= 360.0f || rotation <= -360.0f) rotation = 0.0f;
                m_rotation = rotation;
            }
            Mesh* getMesh() const { return m_mesh; }
            void setMesh(Mesh* mesh) { m_mesh = mesh; }
            Texture* getTexture() const { return m_tex; }
            void setTexture(Texture* tex) { m_tex = tex; }
            
            glm::mat4 getModelMatrix() const;
        
        protected:
            glm::vec2 m_position;
            glm::vec2 m_scale = glm::vec2(1.0f);
            float m_rotation = 0.0f;
            Mesh* m_mesh = nullptr;
            Texture* m_tex = nullptr;
    }; 
} 