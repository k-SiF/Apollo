#include <apollo/window.h>
#include <apollo/shader.h>
#include <apollo/mesh.h>
#include <apollo/entity.h>

// void processInput(GLFWwindow* window, Entity& entity, float deltaTime) {
//     float speed = 0.5f;
//     glm::vec2 pos = entity.getPosition();
//     if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) pos.y += 0.5 * deltaTime;
//     if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) pos.x -= 0.5 * deltaTime;    
//     if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) pos.y -= 0.5 * deltaTime;
//     if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) pos.x += 0.5 * deltaTime;
//     entity.setPosition(pos);
// }

int main() {

    float lastFrame = 0.0f;
   
    Window window(800, 600, "Apollo");
    Shader shader("shaders/triangle.glsl");
    Mesh triangleMesh({ -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f });
    Entity entity(&triangleMesh);

    // Render Loop
    while (!window.shouldClose()) {
        // Delta time setup
        float currentFrame = window.getTime(); // Time since GLFW init
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
    
        // Input
        // processInput(window.handle(), entity, deltaTime);

        // Rendering commands
        window.clear(0.2f, 0.3f, 0.3f, 1.0f); // clear buffer, replace values.

        entity.update(deltaTime);
        shader.use();
        entity.draw(shader);

        // Event call and Buffer swap
        window.swapBuffers();
        window.pollEvents();
    }
    
    return 0;
}