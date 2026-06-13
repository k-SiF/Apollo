#include <apollo/window.h>
#include <apollo/shader.h>
#include <apollo/mesh.h>
#include <apollo/entity.h>
#include <apollo/input.h>
#include <apollo/renderer.h>
#include <iostream>

void processInput(Entity& entity, float deltaTime) {
    float speed = 0.5f;
    glm::vec2 pos = entity.getPosition();
    if (Input::onKeyPress(Key::Up)) pos.y += 0.5 * deltaTime;
    if (Input::onKeyPress(Key::Left)) pos.x -= 0.5 * deltaTime;    
    if (Input::onKeyPress(Key::Down)) pos.y -= 0.5 * deltaTime;
    if (Input::onKeyPress(Key::Right)) pos.x += 0.5 * deltaTime;
    // if (Input::onKeyRelease(Key::Space)) std::cout << "Jump!!" << std::endl;
    entity.setPosition(pos);
}

int main() {

    float lastFrame = 0.0f;
   
    Window window(800, 600, "Apollo");
    Shader shader("shaders/triangle.glsl");
    Mesh triangleMesh({ -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f });
    Entity entity(&triangleMesh);
    Renderer renderer;

    // Render Loop
    while (!window.shouldClose()) {
        // Delta time setup
        float currentFrame = window.getTime(); // Time since GLFW init
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
    
        // Input
        processInput(entity, deltaTime);

        // Rendering commands
        renderer.clear(0.2f, 0.3f, 0.3f, 1.0f); // clear buffer, replace values.
        renderer.begin(shader);

        entity.update(deltaTime);
        renderer.draw(entity);

        // Event call and Buffer swap
        window.swapBuffers();
        window.pollEvents();
    }
    
    return 0;
}