#pragma once
#include <apollo/shader.h>
#include <apollo/entity.h>
#include <apollo/camera.h>
#include <glm/gtc/matrix_transform.hpp> 

namespace apollo {
    class Renderer {
        public:
            Renderer();
            ~Renderer();
            void clear(float r, float g, float b, float a);
            void begin(Shader& shader, int width, int height, const Camera& camera);
            void draw(Entity& entity);
        
        private:
            Shader* m_shader = nullptr;
            glm::mat4 m_projection = glm::mat4(1.0f);
    };
}