#include <apollo/sketch.h>
#include <apollo/chrono.h>
#include <apollo/input.h>
#include <apollo/key.h>
#include <iostream>

namespace apollo {
    Sketch::Sketch() : m_window(800, 600, "Apollo"), m_shader("shaders/triangle.glsl"){

    }

    void Sketch::run() {
        const float FIXED_STEP = 1.0f / 60.0f;
        float accumulator = 0.0f;
        double lastFrame = 0.0f;

        start();
        while (!m_window.shouldClose()) {
            double currentFrame = m_window.getTime(); // Time since GLFW init
            double deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            // --- FPS + step-pattern counter ---
            // static int fc = 0;
            // static double t = 0;
            // fc++;
            // t += deltaTime;
            // if (t >= 1.0) {
            //     std::cout << "FPS: " << fc << "\n";
            //     fc = 0;
            //     t = 0;
            // }
            // ---------------------------------------------

            if (deltaTime > 0.25f) deltaTime = 0.25f; // cap to 25ms
            accumulator += (float)deltaTime;

            // PER-FRAME update
            Time::update(deltaTime);
            update(deltaTime);
            m_scene.update(deltaTime);

            // FIXED update (physics)
            while (accumulator >= FIXED_STEP) {
                Time::fixedUpdate(FIXED_STEP);
                fixedUpdate(FIXED_STEP);
                m_scene.fixedUpdate(FIXED_STEP);
                accumulator -= FIXED_STEP;
            }

            
            float alpha = accumulator / FIXED_STEP; // remaining
            onRender(alpha);

            // RENDER commands
            m_renderer.clear(m_bgR, m_bgG, m_bgR, 1.0f); // clear buffer, replace values.
            m_renderer.begin(m_window.getWidth(), m_window.getHeight(), m_camera);
            m_scene.draw(m_renderer, alpha);

            // EVENT call and Buffer swap
            m_window.swapBuffers();
            m_window.pollEvents();

            if (Input::wasKeyPressed(Key::Esc)) m_window.close();

            // const double TARGET = 1.0 / 120.0;
            // while (m_window.getTime() - currentFrame < TARGET) { } 
        }
    }
}