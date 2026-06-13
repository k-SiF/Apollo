#include <iostream>
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "shader.h"
#include "mesh.h"
#include "window.h"

void processInput(GLFWwindow* window, float& offsetx, float& offsety, float deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) offsety += 0.5 * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) offsetx -= 0.5 * deltaTime;    
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) offsety -= 0.5 * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) offsetx += 0.5 * deltaTime;
}

int main() {

    float lastFrame = 0.0f;
   
    Window window(800, 600, "Apollo");
    Shader shader("shaders/triangle.glsl");
    Mesh triangle({ -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f });

    float offsetX = 0, offsetY = 0;
    // Render Loop
    while (!window.shouldClose()) {
        // Delta time setup
        float currentFrame = static_cast<float>(glfwGetTime()); // Time since GLFW init
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
    
        // Input
        processInput(window.handle(), offsetX, offsetY, deltaTime);

        // Rendering commands
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();
        shader.setVec2("uOffset", offsetX, offsetY);
        triangle.draw();
        

        // Event call and Buffer swap
        window.swapBuffers();
        window.pollEvents();
    }
    
    return 0;
}