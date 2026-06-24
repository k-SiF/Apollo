#pragma once
#include <apollo/shader.h>
#include <apollo/entity.h>
#include <apollo/camera.h>
#include <apollo/font.h>
#include <glm/gtc/matrix_transform.hpp> 

namespace apollo {
    class Renderer {
        public:
            Renderer();
            ~Renderer();
            void clear(float r, float g, float b, float a);
            void begin(int width, int height, const Camera& camera);
            void beginText(int width, int height);
            
            void draw(Entity& entity, float alpha);
            void drawText(const Font& font, const std::string& text, float x, float y, glm::vec3 color = glm::vec3(1.0f));
            void drawDebugBox(const Collider& box, glm::vec3 color = glm::vec3(0.0f, 1.0f, 0.0f));

            bool m_debugDraw = false;
            
        private:
            Shader m_shader;
            Shader m_debugShader;
            Shader m_textShader;

            glm::mat4 m_projection = glm::mat4(1.0f);
            glm::mat4 m_textProjection = glm::mat4(1.0f);
            glm::mat4 m_view = glm::mat4(1.0f);

            unsigned int m_debugVAO = 0;
            unsigned int m_debugVBO = 0;

            unsigned int m_textVAO = 0;
            unsigned int m_textVBO = 0;

    };
}