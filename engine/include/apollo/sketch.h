#pragma once
#include <apollo/window.h>
#include <apollo/renderer.h>
#include <apollo/scene.h>

class Sketch {
    public:
        Sketch();
        virtual ~Sketch() = default;
        void run();

    protected:
        virtual void start() {}
        virtual void update(float deltaTime) = 0;
        void setBackground(float r, float g, float b) { m_bgR = r; m_bgG = g; m_bgB = b; } ;
    
    private:
        Window m_window;
        float m_bgR = 0.2f, m_bgG = 0.3f, m_bgB = 0.3f;

    protected:
        Renderer m_renderer;
        Shader m_shader;
        Scene m_scene;
};