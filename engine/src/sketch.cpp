#include <apollo/sketch.h>
#include <apollo/chrono.h>
#include <apollo/input.h>
#include <apollo/key.h>
#include <iostream>
#include <thread>
#include <chrono>

namespace apollo {
    Sketch::Sketch(const WindowConfig& config) : m_window(config), m_shader("shaders/triangle.glsl"){
        setMaxFPS(config.maxFPS);
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

            // {
            //     static int framesSinceSpike = 0;
            //     framesSinceSpike++;
            //     if (deltaTime > 0.020) {   // >20ms = hitch
            //         std::cout << "SPIKE: " << (deltaTime * 1000.0) << "ms after "
            //                 << framesSinceSpike << " good frames\n";
            //         framesSinceSpike = 0;
            //     }
            // }

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
            m_renderer.clear(m_bgR, m_bgG, m_bgB, 1.0f); // clear buffer, replace values.
            m_renderer.begin(m_window.getWidth(), m_window.getHeight(), m_camera);
            m_scene.draw(m_renderer, alpha);
            drawWorld(alpha);

            // UI/text pass
            m_renderer.beginText(m_window.getWidth(), m_window.getHeight());
            drawUI();

            // EVENT call and Buffer swap
            m_window.swapBuffers();
            m_window.pollEvents();

            if (Input::wasKeyPressed(Key::Escape)) m_window.close();

            // --- frame rate cap (0 = uncapped) ---
            // sleep off most of the leftover time (don't burn a CPU core), then
            // spin the final sub-millisecond for precise pacing.
            if (m_targetFrameTime > 0.0) {
                double frameEnd = currentFrame + m_targetFrameTime;
                double now = m_window.getTime();
                double remaining = frameEnd - now;
                if (remaining > 0.0) {
                    if (remaining > 0.002) {   // sleep all but ~2ms
                        std::this_thread::sleep_for(

                            std::chrono::duration<double>(remaining - 0.002));
                    }
                    // spin the rest for accuracy
                    while (m_window.getTime() < frameEnd) { /* busy-wait briefly */ }

                }

            }
        }
    }
}