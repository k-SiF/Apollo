#pragma once
#include <apollo/shader.h>
#include <apollo/texture.h>
#include <apollo/uvrect.h>
#include <glm/glm.hpp>
#include <vector>

namespace apollo {
    class SpriteBatch {
        public:
            SpriteBatch();
            ~SpriteBatch();

            SpriteBatch(const SpriteBatch&) = delete;
            SpriteBatch& operator=(const SpriteBatch&) = delete;

            // Begin batch with camera transform (view * projection)
            void begin(const glm::mat4& viewProjection);

            // Queue sprite:
            // center = world position of sprite's center
            // size = world width/height
            // uv = sub-rect of the texture (default full)
            void draw(Texture* texture, glm::vec2 center, 
                      glm::vec2 size, const UVRect& uv = UVRect{}, 
                      float rotationDeg = 0.0f, glm::vec4 color = glm::vec4(1.0f));

            // Flush remaining sprites
            void end();

            int drawCallCount() const { return m_drawCalls; }

        private:
            struct Vertex {
                glm::vec2 pos;
                glm::vec2 uv;
                glm::vec4 color;
            };

            static const int MAX_SPRITES = 10000; // per flush
            static const int MAX_VERTICES = MAX_SPRITES * 4;
            static const int MAX_INDICES = MAX_SPRITES * 6;

            // Upload and draw current batch, then reset
            void flush(); 

            Shader m_shader;
            unsigned int m_vao = 0, m_vbo = 0, m_ebo = 0;

            std::vector<Vertex> m_vertices; // on cpu
            Texture* m_currentTexture = nullptr;
            glm::mat4 m_viewProjection = glm::mat4(1.0f);

            int m_drawCalls = 0;
            int m_spritesThisFrame = 0;
    };
}