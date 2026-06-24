#pragma once
#include <apollo/apollo.h>

namespace apollo {
    class Sketch {
        public:
            Sketch();
            virtual ~Sketch() = default;
            void run();

        protected:
            virtual void start() {}
            virtual void update(float deltaTime) = 0;
            virtual void fixedUpdate(float deltaTime) {}
            virtual void onRender(float alpha) {}
            void setBackground(float r, float g, float b) { m_bgR = r; m_bgG = g; m_bgB = b; } ;
            void setMode(WindowMode mode) { m_window.setMode(mode); }
            void toggleFullscreen() { m_window.toggleFullscreen(); }
        
        private:
            Window m_window;
            float m_bgR = 0.2f, m_bgG = 0.3f, m_bgB = 0.3f;

        protected:
            Renderer m_renderer;
            Shader m_shader;
            Scene m_scene;
            Audio m_audio;
            Camera m_camera;
            Resources m_resources;
    };
}