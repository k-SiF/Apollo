#include <apollo/camera.h>
#include <glm/gtc/matrix_transform.hpp>

namespace apollo {
    glm::mat4 Camera::getViewMatrix() const {
        // Return inverse of camera position (world moves opposite to camera)
        return glm::translate(glm::mat4(1.0f), glm::vec3(-m_position, 0.0f));
    }
}