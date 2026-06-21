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
            void begin(int width, int height, const Camera& camera);
            void draw(Entity& entity, float alpha);
            void drawDebugBox(const Collider& box, glm::vec3 color = glm::vec3(0.0f, 1.0f, 0.0f));
        
            bool m_debugDraw = false;
            
        private:
            Shader m_shader;
            Shader m_debugShader;

            glm::mat4 m_projection = glm::mat4(1.0f);
            glm::mat4 m_view = glm::mat4(1.0f);

            unsigned int m_debugVAO = 0;
            unsigned int m_debugVBO = 0;
    };
}