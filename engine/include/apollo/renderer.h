#pragma once
#include <apollo/shader.h>
#include <apollo/entity.h>

class Renderer {
    public:
        Renderer();
        ~Renderer();
        void clear(float r, float g, float b, float a);
        void begin(Shader& shader);
        void draw(Entity& entity);
    
    private:
        Shader* m_shader = nullptr;
};