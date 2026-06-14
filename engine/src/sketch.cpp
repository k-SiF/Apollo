#include "sketch.h"

Sketch::Sketch() : m_window(800, 600, "Apollo"), m_shader("shaders/triangle.glsl"){

}

void Sketch::run() {
    float lastFrame = 0.0f;
    start();
    while (!m_window.shouldClose()) {
        float currentFrame = m_window.getTime(); // Time since GLFW init
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        m_scene.update(deltaTime);

        // Rendering commands
        m_renderer.clear(m_bgR, m_bgG, m_bgR, 1.0f); // clear buffer, replace values.
        m_renderer.begin(m_shader, m_window.getWidth(), m_window.getHeight());
        m_scene.draw(m_renderer);

        // Event call and Buffer swap
        m_window.swapBuffers();
        m_window.pollEvents();
    }
}