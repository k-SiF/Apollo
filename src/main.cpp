#include <iostream>
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "shader.h"
#include "mesh.h"
#include "window.h"

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

int main() {
   
    Window window(800, 600, "Apollo");
    Shader shader("shaders/triangle.glsl");
    Mesh triangle({ -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f });

    // Render Loop
    while (!window.shouldClose()) {
        // Input
        processInput(window.handle());

        // Rendering commands
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();
        triangle.draw();

        // Event call and Buffer swap
        window.swapBuffers();
        window.pollEvents();
    }
    
    return 0;
}