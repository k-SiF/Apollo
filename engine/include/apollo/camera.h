#pragma once
#include <glm/glm.hpp>

namespace apollo {
    class Camera {
        public:
            void setPosition(glm::vec2 pos) { m_position = pos; }
            glm::vec2 getPosition() const { return m_position; }
            void move(glm::vec2 delta) { m_position += delta; }

            glm::mat4 getViewMatrix() const; // for shader uView

        private:
        glm::vec2 m_position = glm::vec2(0.0f);

    };
}