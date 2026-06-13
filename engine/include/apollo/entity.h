#pragma once
#include "mesh.h"
#include "shader.h"

#include <glm/glm.hpp>

class Entity {
    public:
        Entity(Mesh* mesh, glm::vec2 position = glm::vec2(0.0f));
        ~Entity();
        void update(float deltaTime);
        void draw(Shader& shader);

        glm::vec2 getPosition() const { return m_position; }
        void setPosition(glm::vec2 position) { m_position = position; }
        Mesh* getMesh() const { return m_mesh; }
        void setMesh(Mesh* mesh) { m_mesh = mesh; }
    
    private:
        glm::vec2 m_position;
        Mesh* m_mesh;
};